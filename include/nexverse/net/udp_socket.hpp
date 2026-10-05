/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace nexverse::net
{
struct UdpReceiveResult
{
    bool received = false;
    bool timed_out = false;
    std::vector<std::uint8_t> bytes;
    std::string error;
};

class UdpSocket
{
public:
    UdpSocket();
    ~UdpSocket();

    UdpSocket(const UdpSocket&) = delete;
    UdpSocket& operator=(const UdpSocket&) = delete;

    UdpSocket(UdpSocket&& other) noexcept;
    UdpSocket& operator=(UdpSocket&& other) noexcept;

    [[nodiscard]] bool open(std::string_view host, std::uint16_t port, std::string& error);
    [[nodiscard]] bool send(std::span<const std::uint8_t> bytes, std::string& error) const;
    [[nodiscard]] UdpReceiveResult receive(std::size_t max_bytes, std::chrono::milliseconds timeout) const;
    [[nodiscard]] bool is_open() const noexcept;
    void close() noexcept;

private:
#if defined(_WIN32)
    using NativeHandle = std::uintptr_t;
    static constexpr NativeHandle invalid_handle = static_cast<NativeHandle>(~std::uintptr_t{0});
#else
    using NativeHandle = int;
    static constexpr NativeHandle invalid_handle = -1;
#endif

    NativeHandle handle_ = invalid_handle;
};
}
