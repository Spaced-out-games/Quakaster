#include "ComponentLoader.h"


namespace qk {
	bool from_json(const json& j, EntityComponentArgument& arg) {
		if (j.is_null()) {
			return false;
		}
		// don't support object types
		else if (j.is_object()) {
			return false;
		}

		// primitives
		else if (j.is_boolean()) {
			arg = j.get<bool>();
			return true;
		}
		else if (j.is_number_float()) {
			arg = j.get<f32>();
			return true;
		}
		else if (j.is_number_unsigned()) {
			arg = j.get<i32>();
			return true;
		}
		else if (j.is_number_integer()) {
			arg = j.get<u32>();
			return true;
		}
		else if (j.is_string()) {
			arg = j.get<str>();
			return true;
		}

		// arrays
		else if (j.is_array()) {
			JSONArrayClassification classification = classify(j);

			switch (classification)
			{
			case qk::media::JSONArrayClassification::Empty:
				return false;
			case qk::media::JSONArrayClassification::NotAnArray:
				// how the fuck
				return false;
			case qk::media::JSONArrayClassification::Bool:
				arg = j.get<std::vector<bool>>();
				return true;
			case qk::media::JSONArrayClassification::Int:
				arg = j.get<std::vector<i32>>();
				return true;
			case qk::media::JSONArrayClassification::UInt:
				arg = j.get<std::vector<u32>>();
				return true;
			case qk::media::JSONArrayClassification::Float:
				arg = j.get<std::vector<f32>>();
				return true;
				break;
			case qk::media::JSONArrayClassification::String:
				arg = j.get<std::vector<str>>();
				return true;
			case qk::media::JSONArrayClassification::Object:
				// array of objects is unsupported
				return false;
			case qk::media::JSONArrayClassification::Array:
				// array of arrays is unsupported
				return false;
			case qk::media::JSONArrayClassification::Heterogeneous:
				// heterogenous arrays are unsupported
				return false;
			default:
				break;
			}
		}
		return false;

	}

	bool from_json(const json& j, EntityArgumentMap& out) {

		if (!j.is_object()) return false;
		json j_flat;
		flatten_json(j, j_flat);
		EntityComponentArgument arg;
		for (const auto& [k, v] : j_flat.items()) {
			if (from_json(v, arg))
				out[string_hash{ k }] = arg;
			else
				return false;
		}
		return true;


	}
}