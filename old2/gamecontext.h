#pragma once
#include "entt.hpp"
#include "application.h"
#include "server.h"

template <typename app_t = Application>
struct GameContext
{
	entt::dispatcher event_manager;
	app_t app;
};