#pragma once
#include <include/base/Scene.h>


/// <summary>
/// Represents a single broad-phase operation on entities. Useful for modularization. They ARE allowed to store internal state...

/// </summary>
struct ISystem {
	virtual void init(QKScene& scene) = 0;
	virtual void tick(QKScene& scene) = 0;
	virtual void destroy(QKScene& scene) = 0;
};

