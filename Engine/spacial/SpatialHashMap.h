/// **************************************** QUAKASTER ENGINE **************************************** 
/// spacial/SpatialHashMap.h 
/// Purpose: Defines a spacial hash map for optimized spacial queries
/// **************************************************************************************************
#pragma once
#include "core.h"
#include <unordered_map>
#include <vector>
#include <glm/vec3.hpp>

namespace qk::spacial {

	// Defines 
	using SpacialGridCell = glm::ivec3;

	struct SpacialGridCellHash {
		size_t operator()(const SpacialGridCell& cell) const noexcept {

			size_t h1 = std::hash<int>()(cell.x);
			size_t h2 = std::hash<int>()(cell.y);
			size_t h3 = std::hash<int>()(cell.z);

			// decent hash combine
			return h1 ^ (h2 << 1) ^ (h3 << 2);
		}
	};


	template <class value_t, size_t grid_size>
	struct SpatialHashGrid3D {


		std::unordered_map<SpacialGridCell, std::vector<value_t>, SpacialGridCellHash> m_Grid;

		// Get the cell index for a position
		SpacialGridCell cell(const glm::vec3& position) {
			return {
				(int)(std::floor(position.x / grid_size)),
				(int)(std::floor(position.y / grid_size)),
				(int)(std::floor(position.z / grid_size))
			};
		}
		// Insert a value_t at the given position in the spatial hash map
		void insert(const glm::vec3& pos, const value_t& value) {
			m_Grid[cell(pos)].push_back(value);
		}




	};

}