#include "ComponentLoader.h"
#include "fnv1a.h"

using qk::media::array_matches;
using qk::media::json;
namespace qk {
	EntityArgumentMap from_json(const json& j) {
		assert(j.is_object());

		json j_flat;
		media::flatten_json(j, j_flat);
		EntityArgumentMap map;
		for (const auto& [k, v] : j_flat.items()) {

			// just skip it
			if (v.is_null()) continue;
			if (v.is_object()) {
				throw std::runtime_error("object argument types are unsupported.");
			}


			// primitive types
			if (v.is_number_float()) {
				map[string_hash{ k }] = v.get<f32>();
			}
			else if (v.is_number_integer()) {
				map[string_hash{ k }] = v.get<i32>();
			}
			else if (v.is_boolean()) {
				map[string_hash{ k }] = v.get<bool>();
			}
			else if (v.is_string()) {
				map[string_hash{ k }] = v.get<str>();
			}





			// array types
			if (array_matches(v, [](const json& j) {return j.is_number_integer(); }))
			{
				map[string_hash{ k }] = v.get<std::vector<i32>>();
			}
			else if (array_matches(v, [](const json& j) {return j.is_number(); }))
			{
				map[string_hash{ k }] = v.get<std::vector<f32>>();
			}
			else if (array_matches(v, [](const json& j) {return j.is_boolean(); }))
			{
				map[string_hash{ k }] = v.get<std::vector<bool>>();
			}
			else if (array_matches(v, [](const json& j) {return j.is_string(); }))
			{
				map[string_hash{ k }] = v.get<std::vector<str>>();
			}
			else
			{
				throw std::runtime_error("Homogenous array types are unsupported.");
			}
		}

		return map;

	}

	EntityArgumentMap operator+(const EntityArgumentMap& self, const EntityArgumentMap& other) {

		EntityArgumentMap arg_map;

		for (const auto& [k, v] : self) {
			assert(!arg_map.contains(k));
			arg_map[k] = v;
		}

		for (const auto& [k, v] : other) {
			assert(!arg_map.contains(k));
			arg_map[k] = v;
		}

		return arg_map;

	}

}