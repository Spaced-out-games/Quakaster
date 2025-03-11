#pragma once
#include <iostream>
#include <chrono>
#include <include/thirdparty/entt.hpp>

struct EventHandler {};

struct Event {
    Event()
        : timestamp(std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch())) {
        //std::cout << timestamp.count() << " microseconds since epoch\n";
    }

    template <typename T, typename... Args>
    static void fire(Args&&... args) {
        dispatcher.trigger<T>(T(std::forward<Args>(args)...)); // Forward arguments to the dispatcher
    }

    template <typename event_t, typename invoker_t, auto func>
    static void subscribe(invoker_t* this_ptr) {
        dispatcher.sink<event_t>().connect<func>(this_ptr);
    }

        static inline entt::dispatcher dispatcher;
    private:

        std::chrono::microseconds timestamp;
};

