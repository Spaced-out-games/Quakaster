#pragma once
#include <include/GameContext/base/Scene.h>

namespace Quakaster::base {
	/// <summary>
	/// Represents a single broad-phase operation on entities. Useful for modularization. They ARE allowed to store internal state...

	/// </summary>
	struct ISystem {
		virtual void init(Scene& scene) = 0;
		virtual void tick(Scene& scene) = 0;
		virtual void destroy(Scene& scene) = 0;
	};
}


using ISystem = Quakaster::base::ISystem;