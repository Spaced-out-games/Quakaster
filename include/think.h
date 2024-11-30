#pragma once
#include "utility_types.h"
#include <chrono>

class Scene;



// Script to be executed on a set interval in terms of a Scene. 
class thinkScript
{
public:
    // Constructor. Defaults to 60 tps by default
    thinkScript(func_ptr_t<void, Scene&> func, int tps)
        : function(func),
        tick_interval(std::chrono::microseconds(1000000 / tps)),
        next_tick(std::chrono::high_resolution_clock::now() + std::chrono::microseconds(tick_interval))
    {
    }
    thinkScript(func_ptr_t<void, Scene&> func)
        : function(func),
        tick_interval(std::chrono::microseconds(0)),
        next_tick(std::chrono::high_resolution_clock::now() + std::chrono::microseconds(tick_interval))
    {
    }

    // Tick interval in microseconds
    std::chrono::microseconds tick_interval;

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

    // Set the tick interval (in microseconds)
    void setTickInterval(int us)
    {
        tick_interval = std::chrono::microseconds(us);
    }
    // Manually set the next tick time. This is great if you want to set a unique interval for a single interval then have it revert
    void advanceTickManual(int us)
    {
        next_tick += std::chrono::microseconds(us);
    }
};