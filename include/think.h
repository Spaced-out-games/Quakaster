#pragma once
#include "utility_types.h"
#include <chrono>

class Scene;

class thinkScript
{
public:
    // Constructor
    thinkScript(func_ptr_t<void, Scene&> func, int ticks_per_second = 60)
        : function(func),
        tick_interval(std::chrono::milliseconds(1000 / ticks_per_second)),
        next_tick(std::chrono::high_resolution_clock::now() + tick_interval)
    {
    }

    // Tick interval and next tick
    std::chrono::milliseconds tick_interval;
    std::chrono::time_point<std::chrono::high_resolution_clock> next_tick;

    // Function to execute
    func_ptr_t<void, Scene&> function;

    // Operator to execute the function
    inline void operator()(Scene& scene)
    {
        function(scene);
    }

    // Check if the script is ready to tick
    bool shouldTick() const
    {
        return std::chrono::high_resolution_clock::now() >= next_tick;
    }

    // Advance to the next tick
    void advanceTick()
    {
        next_tick += tick_interval;
    }
};