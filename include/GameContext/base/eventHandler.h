#pragma once
#include <include/thirdparty/entt.hpp>

namespace Quakaster::base {


	struct EventHandler : public entt::dispatcher {
		// Nothing special
	};


}

using EventHandler = Quakaster::base::EventHandler;