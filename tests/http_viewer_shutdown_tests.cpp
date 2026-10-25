/* MPL-2.0. Loopback regression: logout/window close cancels stalled
 * authenticated CAPS-style HTTP jobs instead of waiting the timeout.
 */
#include "nexverse/net/http_transport.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <thread>
using namespace std::chrono_literals;

namespace
{
struct SlowServer
{
    int socket_fd=-1;
    std::uint16_t port=0;
    std::atomic<bool> request_seen{false};
    std::atomic<bool> end{false};
    std::thread worker;
    SlowServer()
    {
        socket_fd=::socket(AF_INET,SOCK_STREAM,0);
        if(socket_fd<0)return;
        int reuse=1;
        (void)::setsockopt(socket_fd,SOL_SOCKET,SO_REUSEADDR,
                           &reuse,sizeof(reuse));
        sockaddr_in bind_addr{};
        bind_addr.sin_family=AF_INET;
        bind_addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
        if(::bind(socket_fd,reinterpret_cast<sockaddr*>(&bind_addr),
                  sizeof(bind_addr))!=0||::listen(socket_fd,2)!=0)return;
        socklen_t length=sizeof(bind_addr);
        if(::getsockname(socket_fd,reinterpret_cast<sockaddr*>(&bind_addr),
                         &length)!=0)return;
        port=ntohs(bind_addr.sin_port);
        worker=std::thread([this]{
            fd_set ready{};
            FD_ZERO(&ready);
            FD_SET(socket_fd,&ready);
            timeval tv{3,0};
            if(::select(socket_fd+1,&ready,nullptr,nullptr,&tv)!=1)return;
            const int client=::accept(socket_fd,nullptr,nullptr);
            if(client<0)return;
            char data[512]{};
            (void)::recv(client,data,sizeof(data),0);
            const std::string header=
                "HTTP/1.1 200 OK\r\nContent-Type: application/llsd+xml\r\n"
                "Content-Length: 100000\r\nConnection: close\r\n\r\n"
                "<llsd><map>";
            (void)::send(client,header.data(),header.size(),0);
            request_seen.store(true,std::memory_order_release);
            // Deliberately hold the response open; the real application
            // must quit without waiting this stalled HTTP job's timeout.
            for(int i=0;i<500 && !end.load();++i)
                std::this_thread::sleep_for(10ms);
            ::shutdown(client,SHUT_RDWR);
            ::close(client);
        });
    }
    ~SlowServer()
    {
        end.store(true);
        if(worker.joinable())worker.join();
        if(socket_fd>=0)::close(socket_fd);
    }
};
}

int main()
{
    nexverse::net::CurlHttpTransport closed;
    closed.request_stop();
    const auto rejected=closed.get(
        "http://127.0.0.1:1/never-try",1024U,500ms,4500ms);
    if(rejected.ok()||rejected.error.find("cancelled")==std::string::npos)
        return 1;
    SlowServer server;
    if(server.port==0)return 2;
    nexverse::net::CurlHttpTransport transport;
    const auto url="http://127.0.0.1:"+
        std::to_string(server.port)+"/EventQueueGet";
    auto future=std::async(std::launch::async,[&]{
        return transport.get(url,100000U,500ms,4500ms);
    });
    const auto ready_start=std::chrono::steady_clock::now();
    while(!server.request_seen.load(std::memory_order_acquire) &&
          std::chrono::steady_clock::now()-ready_start<2s)
        std::this_thread::sleep_for(5ms);
    if(!server.request_seen.load())return 3;
    const auto stopped=std::chrono::steady_clock::now();
    transport.request_stop();
    const bool quick=future.wait_for(2500ms)==std::future_status::ready;
    const auto response=future.get();
    const auto elapsed=std::chrono::duration_cast<
        std::chrono::milliseconds>(std::chrono::steady_clock::now()-stopped);
    if(!quick||response.ok()||
       response.error.find("cancelled")==std::string::npos)
    {
        std::cerr<<"Transport HTTP shutdown not fast enough: "<<
            elapsed.count()<<" ms, status="<<response.status_code<<"\n";
        return 4;
    }
    std::cout<<"OGL_HTTP_VIEWER_SHUTDOWN_CANCEL_PASS "<<
        elapsed.count()<<"ms vs blocked HTTP 4500ms timeout\n";
    return 0;
}
