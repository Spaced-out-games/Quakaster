#pragma once
#include <thirdparty\entt\single_include\entt\entt.hpp>
#include <iostream>
static entt::registry scene;

#define BIT(n) (1ul << n)

enum class event_enum : uint32_t
{
    NONE = 0,

    // window stuff
    WINDOW_OPEN = 1 << 0,
    WINDOW_CLOSE = 1 << 1,
    WINDOW_RESIZE = 1 << 2,
    WINDOW_MINIMIZE = 1 << 3,
    WINDOW_MAXIMIZE = 1 << 4,

    // mouse stuff
    MOUSE_BEGIN_MOVE = 1 << 5,
    MOUSE_MOTION = 1 << 6,
    MOUSE_END_MOVE = 1 << 7,
    MOUSE_BUTTON_CLICK = 1 << 8,
    MOUSE_BUTTON_REPEAT = 1 << 9,
    MOUSE_BUTTON_RELEASE = 1 << 10,
    MOUSE_SCROLL_WHEEL = 1 << 11,

    // keyboard stuff
    KEY_PRESS = 1 << 12,
    KEY_REPEAT = 1 << 13,
    KEY_RELEASE = 1 << 14,

    // Entity events
    ENT_EVENT_FIRE = 1 << 15,
    ENT_EVENT_RECEIVE = 1 << 16,
    ENT_KILL = 1 << 17,
    ENT_SPAWN = 1 << 18,

    // Scene events
    SCENE_RELOAD = 1 << 19,
    SCENE_DESTROY = 1 << 20,

    // GUI events
    GUI_GAIN_FOCUS = 1 << 21,
    GUI_LOSE_FOCUS = 1 << 22,

    // Future events can use bits 23 and higher
};



// Basic event type
struct Event
{
	entt::entity target;
	event_enum event_type; // temporary
	void* details;
};



template <typename return_t, typename... arg_t>
using func_ptr_t = return_t(*)(arg_t...);

using tick_function = func_ptr_t<void, void>;
using on_event_function = func_ptr_t<void, Event>;
using to_string_function = func_ptr_t<std::string, void>;

void on_tick_default() {}
void on_event_default(Event e) {
    std::cout << "on_event called on entity: " << entt::to_integral(e.target);
}


std::string to_string_default() { return ""; }


// A wrapper for entt::entity. It is exactly the same as an entt::entity, with some extra functionality
entt::entity entity()
{
	entt::entity ent = scene.create();
	scene.emplace<tick_function>(ent, &on_tick_default);
	scene.emplace<on_event_function>(ent, &on_event_default);
	return ent;
}



