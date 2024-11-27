#pragma once
#include "utility_types.h"
#include <chrono>

class Scene;


// Script to be executed on a set interval in terms of a Scene. 
class thinkScript
{
public:
    // Constructor
    thinkScript(func_ptr_t<void, Scene&> func, int ticks_per_second = 600)
        : function(func),
        tick_interval(std::chrono::milliseconds(1000 / ticks_per_second)),
        next_tick(std::chrono::high_resolution_clock::now() + tick_interval)
    {
    }

    // Tick interval in milliseconds
    std::chrono::milliseconds tick_interval;

    // time of the next tick
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

    // Set the tick interval
    void setTickInterval(int ms)
    {
        tick_interval = std::chrono::milliseconds(ms);
    }
    // Manually set the next tick time. This is great if you want to set a unique interval for a single interval then have it revert
    void advanceTickManual(int ms)
    {
        next_tick += std::chrono::milliseconds(ms);
    }
};