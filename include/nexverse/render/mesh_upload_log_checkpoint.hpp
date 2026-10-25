/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <cstdint>
namespace nexverse::render
{
// Successful mesh uploads remain real; only their INFO log frequency is
// bounded. Log first and every 128th upload, preserving failure logs.
[[nodiscard]] constexpr bool mesh_upload_log_checkpoint(
    std::uint64_t successful_uploads) noexcept
{
    return successful_uploads==1U ||
           (successful_uploads>0U && successful_uploads%128U==0U);
}
}
