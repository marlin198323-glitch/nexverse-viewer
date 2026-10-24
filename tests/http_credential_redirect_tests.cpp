/* MPL-2.0. Real loopback libcurl transport redirect regression. */
#include "nexverse/net/http_transport.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
using namespace std::chrono_literals;
namespace
{
struct OnceServer
{
    int fd=-1;
    std::uint16_t port=0;
    int requests=0;
    bool reached_redirect=false;
    std::thread worker;
    OnceServer()
    {
        fd=::socket(AF_INET,SOCK_STREAM,0);
        if(fd<0)return;
        int reuse=1;
        ::setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
        sockaddr_in addr{};
        addr.sin_family=AF_INET;
        addr.sin_port=0;
        addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
        if(::bind(fd,reinterpret_cast<sockaddr*>(&addr),sizeof(addr))!=0||
           ::listen(fd,2)!=0)
            return;
        socklen_t length=sizeof(addr);
        if(::getsockname(fd,reinterpret_cast<sockaddr*>(&addr),&length)!=0)
            return;
        port=ntohs(addr.sin_port);
        worker=std::thread([this]{
            for(int i=0;i<2;++i)
            {
                fd_set read{};
                FD_ZERO(&read);FD_SET(fd,&read);
                timeval wait{i==0?3L:0L,i==0?0L:350000L};
                if(::select(fd+1,&read,nullptr,nullptr,&wait)!=1)break;
                const int client=::accept(fd,nullptr,nullptr);
                if(client<0)break;
                ++requests;
                char buffer[1024]={};
                const auto n=::recv(client,buffer,sizeof(buffer),0);
                if(n>0&&std::string_view(buffer,static_cast<std::size_t>(n))
                    .find("/stolen")!=std::string_view::npos)
                    reached_redirect=true;
                const std::string answer=(i==0)
                    ?"HTTP/1.1 307 Temporary Redirect\r\nLocation: http://127.0.0.1:"+
                      std::to_string(port)+"/stolen\r\nContent-Length: 0\r\n"
                      "Connection: close\r\n\r\n"
                    :"HTTP/1.1 200 OK\r\nContent-Length: 2\r\n"
                      "Connection: close\r\n\r\nOK";
                (void)::send(client,answer.data(),answer.size(),0);
                ::shutdown(client,SHUT_RDWR);
                ::close(client);
            }
        });
    }
    ~OnceServer()
    {
        if(worker.joinable())worker.join();
        if(fd>=0)::close(fd);
    }
};
}
int main()
{
    using nexverse::net::CurlHttpTransport;
    CurlHttpTransport transport;
    {
        OnceServer server;
        if(server.port==0)return 1;
        const auto url="http://127.0.0.1:"+std::to_string(server.port)+"/login";
        const auto result=transport.post(
            url,"text/xml","<secret>hashed-avatar-password</secret>",
            1024U,500ms,2500ms);
        if(result.status_code!=307||result.ok())return 2;
        if(server.worker.joinable())server.worker.join();
        if(server.requests!=1||server.reached_redirect)return 3;
    }
    {
        OnceServer server;
        if(server.port==0)return 4;
        const auto url="http://127.0.0.1:"+std::to_string(server.port)+
            "/caps?bearer=SENSITIVE_TOKEN";
        const auto result=transport.get(url,1024U,500ms,2500ms);
        if(result.status_code!=307||result.ok())return 5;
        if(server.worker.joinable())server.worker.join();
        if(server.requests!=1||server.reached_redirect)return 6;
    }
    std::cout<<"OGL_LIBCURL_AUTH_POST_AND_TOKEN_GET_REDIRECT_NOT_FOLLOWED_PASS\n";
}
