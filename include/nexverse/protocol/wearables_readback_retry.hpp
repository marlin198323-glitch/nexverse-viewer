/* MPL-2.0 */
#pragma once
#include <chrono>
#include <cstdint>
namespace nexverse::protocol {
class WearablesReadbackRetry {
public:
 using Clock=std::chrono::steady_clock;
 void note_initial_sent(Clock::time_point now) noexcept
 { if(first_==Clock::time_point{}) first_=now; }
 void note_response() noexcept { replied_=true; }
 bool due(Clock::time_point now) const noexcept
 { return first_!=Clock::time_point{}&&!replied_&&count_<2U&&
   now>=first_&&now-first_>=(count_==0U?
     std::chrono::seconds{5}:std::chrono::seconds{12}); }
 void note_retry_sent() noexcept { if(count_<2U)++count_; }
 std::uint32_t retries() const noexcept {return count_;}
 bool received() const noexcept {return replied_;}
private:
 Clock::time_point first_{};
 std::uint32_t count_=0U;
 bool replied_=false;
};
}
