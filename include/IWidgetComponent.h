#pragma once
#include "entt.hpp"
#include <imgui.h>
#include <string>
#include <iostream>

// Define func_ptr_t as before
template <typename return_t, typename... arg_t>
using func_ptr_t = return_t(*)(arg_t...);

struct IWidgetComponent
{
public:
    //IWidgetComponent(): draw_func(nullptr) {}
    IWidgetComponent(func_ptr_t<void, IWidgetComponent*> draw_func)
        : draw_func(draw_func) {}

    void draw() { draw_func(this); }

    // Convenience function to downcast
    template<typename T>
    T& cast()
    {
        static_assert(std::is_base_of<IWidgetComponent, T>::value, "Attempted to cast to a type not derivative of IWidgetComponent");
        return *(T*)(this);

    }

protected:
    func_ptr_t<void, IWidgetComponent*> draw_func = nullptr; // Pointer to the draw function
    

private:
    static void draw_impl(IWidgetComponent* component) {}
};


