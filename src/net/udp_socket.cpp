/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/net/udp_socket.hpp"

#include <algorithm>
#include <array>
#include <cstring>

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace
{
#if defined(_WIN32)
bool ensure_winsock(std::string& error)
{
    static bool initialized = false;
    static bool attempted = false;
    if (!attempted)
    {
        attempted = true;
        WSADATA data{};
        initialized = WSAStartup(MAKEWORD(2, 2), &data) == 0;
    }
    if (!initialized)
    {
        error = "WSAStartup failed";
    }
    return initialized;
}
#endif
}

namespace nexverse::net
{
UdpSocket::UdpSocket() = default;

UdpSocket::~UdpSocket()
{
    close();
}

UdpSocket::UdpSocket(UdpSocket&& other) noexcept
    : handle_{other.handle_}
{
    other.handle_ = invalid_handle;
}

UdpSocket& UdpSocket::operator=(UdpSocket&& other) noexcept
{
    if (this != &other)
    {
        close();
        handle_ = other.handle_;
        other.handle_ = invalid_handle;
    }
    return *this;
}

bool UdpSocket::open(std::string_view host, std::uint16_t port, std::string& error)
{
    close();

#if defined(_WIN32)
    if (!ensure_winsock(error)) return false;
#endif

    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    addrinfo* result = nullptr;
    const std::string host_copy{host};
    const std::string service = std::to_string(port);

    const int resolved = getaddrinfo(host_copy.c_str(), service.c_str(), &hints, &result);
    if (resolved != 0 || result == nullptr)
    {
#if defined(_WIN32)
        error = "UDP address resolution failed";
#else
        error = std::string{"UDP address resolution failed: "} + gai_strerror(resolved);
#endif
        return false;
    }

    bool connected = false;
    for (addrinfo* current = result; current != nullptr; current = current->ai_next)
    {
#if defined(_WIN32)
        SOCKET candidate = socket(current->ai_family, current->ai_socktype, current->ai_protocol);
        if (candidate == INVALID_SOCKET) continue;
        if (::connect(candidate, current->ai_addr, static_cast<int>(current->ai_addrlen)) == 0)
        {
            handle_ = static_cast<NativeHandle>(candidate);
            connected = true;
            break;
        }
        closesocket(candidate);
#else
        const int candidate = socket(current->ai_family, current->ai_socktype, current->ai_protocol);
        if (candidate < 0) continue;
        if (::connect(candidate, current->ai_addr, current->ai_addrlen) == 0)
        {
            handle_ = candidate;
            connected = true;
            break;
        }
        ::close(candidate);
#endif
    }

    freeaddrinfo(result);

    if (!connected)
    {
        error = "UDP socket could not connect to simulator";
        return false;
    }
    return true;
}

bool UdpSocket::send(std::span<const std::uint8_t> bytes, std::string& error) const
{
    if (!is_open())
    {
        error = "UDP socket is not open";
        return false;
    }
    if (bytes.empty())
    {
        error = "Refusing to send empty UDP datagram";
        return false;
    }

#if defined(_WIN32)
    const int sent = ::send(
        static_cast<SOCKET>(handle_),
        reinterpret_cast<const char*>(bytes.data()),
        static_cast<int>(bytes.size()),
        0);
    if (sent != static_cast<int>(bytes.size()))
#else
    const ssize_t sent = ::send(handle_, bytes.data(), bytes.size(), 0);
    if (sent != static_cast<ssize_t>(bytes.size()))
#endif
    {
        error = "UDP send failed";
        return false;
    }
    return true;
}

UdpReceiveResult UdpSocket::receive(
    std::size_t max_bytes,
    std::chrono::milliseconds timeout) const
{
    UdpReceiveResult result;
    if (!is_open())
    {
        result.error = "UDP socket is not open";
        return result;
    }

    max_bytes = std::clamp<std::size_t>(max_bytes, 1U, 65535U);

#if defined(_WIN32)
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(static_cast<SOCKET>(handle_), &read_set);

    timeval tv{};
    tv.tv_sec = static_cast<long>(timeout.count() / 1000);
    tv.tv_usec = static_cast<long>((timeout.count() % 1000) * 1000);

    const int ready = select(0, &read_set, nullptr, nullptr, &tv);
    if (ready == 0)
    {
        result.timed_out = true;
        return result;
    }
    if (ready < 0)
    {
        result.error = "UDP select failed";
        return result;
    }
#else
    pollfd descriptor{};
    descriptor.fd = handle_;
    descriptor.events = POLLIN;

    const int ready = poll(&descriptor, 1, static_cast<int>(timeout.count()));
    if (ready == 0)
    {
        result.timed_out = true;
        return result;
    }
    if (ready < 0)
    {
        result.error = "UDP poll failed";
        return result;
    }
#endif

    result.bytes.resize(max_bytes);

#if defined(_WIN32)
    const int received = ::recv(
        static_cast<SOCKET>(handle_),
        reinterpret_cast<char*>(result.bytes.data()),
        static_cast<int>(result.bytes.size()),
        0);
    if (received <= 0)
#else
    const ssize_t received = ::recv(handle_, result.bytes.data(), result.bytes.size(), 0);
    if (received <= 0)
#endif
    {
        result.bytes.clear();
        result.error = "UDP receive failed";
        return result;
    }

    result.bytes.resize(static_cast<std::size_t>(received));
    result.received = true;
    return result;
}

bool UdpSocket::is_open() const noexcept
{
    return handle_ != invalid_handle;
}

void UdpSocket::close() noexcept
{
    if (!is_open()) return;

#if defined(_WIN32)
    closesocket(static_cast<SOCKET>(handle_));
#else
    ::close(handle_);
#endif
    handle_ = invalid_handle;
}
}
