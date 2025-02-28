#pragma once
#include <include/GameContext/base/Scene.h>

namespace Quakaster::base {
	/// <summary>
	/// Represents a single broad-phase operation on entities. Useful for modularization. They are not meant to store the state of anything...
	/// If you need internal state, create your own custom structure, and add it as a component. You can do this via init(Scene&), calling get_universal_state_entity(), then appending your component. Of course, the tick() and destroy() functions can function likewise
	/// </summary>
	struct ISystem {
		virtual void init(Scene& scene) = 0;
		virtual void tick(Scene& scene) = 0;
		virtual void destroy(Scene& scene) = 0;
	};
}

