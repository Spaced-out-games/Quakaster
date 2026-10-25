#pragma once
#include <variant>
#include <vector>
#include <core.h>
#include <optional>
#include "../vendor/emhash/hash_table5.hpp"
#include <media/json.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


namespace qk {

	using media::json;

	// supported variant types
	using KeyValues_SupportedTypes = type_list<
		bool, i32, u32, f32, str,
		std::vector<bool>,
		std::vector<i32>,
		std::vector<u32>,
		std::vector<f32>,
		std::vector<str>
	>;

	// key type
	using KeyValues_Key_t = str;

	using KeyValuesValueStorage_t = UnpackTypeList<std::variant, KeyValues_SupportedTypes>::type;

	// value type
	struct Value : KeyValuesValueStorage_t {
		using KeyValuesValueStorage_t::KeyValuesValueStorage_t;

		// tries to cast to T for obvious case
		template <class T>
		std::optional<T> as() const {
			static_assert(contains_T<T, KeyValues_SupportedTypes>);

			if (const T* pValue = std::get_if<T>(this))
				return *pValue;

			return std::nullopt;

		}


		template <>
		std::optional<glm::vec2> as() const {

			if (const auto* pValue = std::get_if<std::vector<f32>>(this))
				if (pValue->size() == 2) return glm::vec2{ pValue->at(0), pValue->at(1) };
				else if (const auto* pValue = std::get_if<std::vector<i32>>(this))
					if (pValue->size() == 2) return glm::vec2{ (f32)pValue->at(0), (f32)pValue->at(1) };
			return std::nullopt;

		}

		template <>
		std::optional<glm::vec3> as() const {

			if (const auto* pValue = std::get_if<std::vector<f32>>(this))
				if (pValue->size() == 3) return glm::vec3{ pValue->at(0), pValue->at(1), pValue->at(2) };
				else if (const auto* pValue = std::get_if<std::vector<i32>>(this))
					if (pValue->size() == 3) return glm::vec3{ (f32)pValue->at(0),(f32)pValue->at(1), (f32)pValue->at(3) };

			return std::nullopt;

		}

		template <>
		std::optional<glm::vec4> as() const {

			if (const auto* pValue = std::get_if<std::vector<f32>>(this))
				if (pValue->size() == 4) return glm::vec4{ pValue->at(0), pValue->at(1), pValue->at(2), pValue->at(3) };

				else if (const auto* pValue = std::get_if<std::vector<i32>>(this))
					if (pValue->size() == 4) return glm::vec4{ (f32)pValue->at(0), (f32)pValue->at(1), (f32)pValue->at(2), (f32)pValue->at(3) };

			return std::nullopt;

		}

		template <>
		std::optional<glm::quat> as() const {

			if (const auto* pValue = std::get_if<std::vector<f32>>(this))
				if (pValue->size() == 4) return glm::quat{ pValue->at(0), pValue->at(1), pValue->at(2), pValue->at(3) };

				else if (const auto* pValue = std::get_if<std::vector<i32>>(this))
					if (pValue->size() == 4) return glm::quat{ (f32)pValue->at(0), (f32)pValue->at(1), (f32)pValue->at(2), (f32)pValue->at(3) };

			return std::nullopt;

		}




	};


	using KeyValues = emhash5::HashMap<KeyValues_Key_t, Value>;

	std::optional<Value> KeyValueScalar_fromJSON(const json& j);

	// array of scalars -> std::vector<T>; fails on null or mixed bool/string/number
	std::optional<Value> KeyValueArray_fromJSON(const json& arr);

	bool KeyValues_fromJSON(const json& j, const str& key, KeyValues& out, char delimiter);




}