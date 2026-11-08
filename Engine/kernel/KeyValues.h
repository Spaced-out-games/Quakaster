
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
#include <expected>

namespace qk {

    using media::json;

    using KeyValuesPrimitives = type_list<bool, i32, u32, f32, str>;

    using KeyValuesVectors =
        transform_type_list_t<std::vector, KeyValuesPrimitives>;

    using KeyValuesSupportedTypes = concat_type_lists_t<
        KeyValuesPrimitives,
        KeyValuesVectors
    >;

    enum class KeyValuesParseError : u8 {
        UnsupportedType,
        MixedArrayTypes,
    };

    // Key type
    using KeyValues_Key_t = str;

    // Value storage
    using KeyValuesValueStorage_t =
        apply_type_list_t<std::variant, KeyValuesSupportedTypes>;

    // Value type
    struct Value : KeyValuesValueStorage_t {

        using KeyValuesValueStorage_t::KeyValuesValueStorage_t;

        // Tries to cast to T for obvious cases.
        template <class T>
        std::optional<T> as() const {
            static_assert(contains_type_v<T, KeyValuesSupportedTypes>);

            if (const T* pValue = std::get_if<T>(this))
                return *pValue;

            return std::nullopt;
        }

        template <>
        std::optional<glm::vec2> as() const {

            if (const auto* pValue = std::get_if<std::vector<f32>>(this)) {
                if (pValue->size() == 2)
                    return glm::vec2{
                        pValue->at(0),
                        pValue->at(1)
                };
            }
            else if (const auto* pValue = std::get_if<std::vector<i32>>(this)) {
                if (pValue->size() == 2)
                    return glm::vec2{
                        static_cast<f32>(pValue->at(0)),
                        static_cast<f32>(pValue->at(1))
                };
            }

            return std::nullopt;
        }

        template <>
        std::optional<glm::vec3> as() const {

            if (const auto* pValue = std::get_if<std::vector<f32>>(this)) {
                if (pValue->size() == 3)
                    return glm::vec3{
                        pValue->at(0),
                        pValue->at(1),
                        pValue->at(2)
                };
            }
            else if (const auto* pValue = std::get_if<std::vector<i32>>(this)) {
                if (pValue->size() == 3)
                    return glm::vec3{
                        static_cast<f32>(pValue->at(0)),
                        static_cast<f32>(pValue->at(1)),
                        static_cast<f32>(pValue->at(2))
                };
            }

            return std::nullopt;
        }

        template <>
        std::optional<glm::vec4> as() const {

            if (const auto* pValue = std::get_if<std::vector<f32>>(this)) {
                if (pValue->size() == 4)
                    return glm::vec4{
                        pValue->at(0),
                        pValue->at(1),
                        pValue->at(2),
                        pValue->at(3)
                };
            }
            else if (const auto* pValue = std::get_if<std::vector<i32>>(this)) {
                if (pValue->size() == 4)
                    return glm::vec4{
                        static_cast<f32>(pValue->at(0)),
                        static_cast<f32>(pValue->at(1)),
                        static_cast<f32>(pValue->at(2)),
                        static_cast<f32>(pValue->at(3))
                };
            }

            return std::nullopt;
        }

        template <>
        std::optional<glm::quat> as() const {

            if (const auto* pValue = std::get_if<std::vector<f32>>(this)) {
                if (pValue->size() == 4)
                    return glm::quat{
                        pValue->at(0),
                        pValue->at(1),
                        pValue->at(2),
                        pValue->at(3)
                };
            }
            else if (const auto* pValue = std::get_if<std::vector<i32>>(this)) {
                if (pValue->size() == 4)
                    return glm::quat{
                        static_cast<f32>(pValue->at(0)),
                        static_cast<f32>(pValue->at(1)),
                        static_cast<f32>(pValue->at(2)),
                        static_cast<f32>(pValue->at(3))
                };
            }

            return std::nullopt;
        }
    };

    using KeyValues = emhash5::HashMap<KeyValues_Key_t, Value>;

    std::expected<Value, KeyValuesParseError>
        KeyValueScalar_fromJSON(const json& j);

    std::expected<Value, KeyValuesParseError>
        KeyValueArray_fromJSON(const json& arr);

    std::expected<void, KeyValuesParseError>
        KeyValues_fromJSON(
            const json& j,
            const str& key,
            KeyValues& out,
            char delimiter = '.'
        );

}
