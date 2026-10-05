#include "nexverse/protocol/caps_event_dispatcher.hpp"

#include <iostream>

int main()
{
    using namespace nexverse::protocol;

    CapsEventDispatcher dispatcher;
    int enable_calls = 0;
    int fallback_calls = 0;

    dispatcher.register_handler(
        "EnableSimulator",
        [&](const EventQueueEvent& event)
        {
            if (event.message == "EnableSimulator")
            {
                ++enable_calls;
            }
        });

    dispatcher.set_fallback_handler(
        [&](const EventQueueEvent&)
        {
            ++fallback_calls;
        });

    EventQueueEvent enable;
    enable.message = "EnableSimulator";
    enable.body = LlsdValue::map();

    if (!dispatcher.dispatch(enable) ||
        enable_calls != 1 ||
        fallback_calls != 0 ||
        dispatcher.handler_count() != 1U)
    {
        std::cerr << "CAPS event handler failed\n";
        return 1;
    }

    EventQueueEvent unknown;
    unknown.message = "UnknownEvent";
    unknown.body = LlsdValue::map();

    if (!dispatcher.dispatch(unknown) || fallback_calls != 1)
    {
        std::cerr << "CAPS event fallback failed\n";
        return 2;
    }

    dispatcher.clear_handler("EnableSimulator");
    if (dispatcher.has_handler("EnableSimulator"))
    {
        std::cerr << "CAPS event handler clear failed\n";
        return 3;
    }

    std::cout << "CAPS event dispatcher tests passed\n";
    return 0;
}
