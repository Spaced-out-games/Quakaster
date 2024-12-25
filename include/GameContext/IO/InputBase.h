#pragma once
#include <SDL.h>

// Abstract base class that processes an SDL_Event in some way
struct InputBase
{
    virtual void on_event(const SDL_Event& event) = 0;
    virtual ~InputBase() = default;
};