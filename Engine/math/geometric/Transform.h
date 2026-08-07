#pragma once
#include "Vec3.h"
#include "Quat.h"
#include "Mat4.h"

namespace qk {




	struct Transform {
		Quat rotation;
		Vec3 position;
		Vec3 scale;

		// not implemented yet!
		explicit operator Mat4() {
			return compose(position, rotation, scale);
		}
	};

	using CTransform = Transform;


}