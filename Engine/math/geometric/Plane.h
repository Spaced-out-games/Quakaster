#pragma once
#include <core.h>	

namespace qk {

	
	template <class Vector_t>
		requires requires(Vector_t a, Vector_t b)
		{
			dot(a, b);
			a - b;
			1.0f * a;
		}
	struct Hyperplane {

		enum class PointClassification : u8 { Back, Coplanar, Front };

		Vector_t normal;
		float distance;

		float signedDistance(const Vector_t& point) const { return Dot(normal, point) - distance; }

		PointClassification classify(const Vector_t& point) {
			float s = signedDistance(point);
			if (s > QK_EPSILON)
				return PointClassification::Front;
			if (s < -QK_EPSILON)
				return PointClassification::Back;
			
			return PointClassification::Coplanar;
		}


	};

	

	template <class Vector_t>
	Vector_t plane_project(const Hyperplane<Vector_t>& plane, const Vector_t& point) {
		float s = plane.signedDistance(point);
		return point - s * plane.normal;
	}

	template <class Vector_t>
	float plane_intersect(const Hyperplane<Vector_t>& plane, const Vector_t& origin, const Vector_t& direction) {
		float denom = dot(plane.normal, direction);
		// if the plane's normal and the direction vector are perpendicular, there's never a hit so we return t = FLT_MAX as a sentinel
		if (abs(denom) < QK_EPSILON)
			return FLT_MAX;
		return (plane.distance - dot(plane.normal, origin)) / denom;
	}



	//Vector_t project(const Vector_t& point) {

	//}


}