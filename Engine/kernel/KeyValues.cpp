#include "KeyValues.h"

namespace qk {

    std::expected<Value, KeyValuesParseError>
        KeyValueScalar_fromJSON(const json& j) {
        if (j.is_boolean())
            return Value{ j.get<bool>() };

        if (j.is_number_integer()) {
            if (j.is_number_unsigned()) {
                const auto v = j.get<uint64_t>();

                if (v <= (uint64_t)std::numeric_limits<i32>::max())
                    return Value{
                        std::in_place_type<i32>,
                        (i32)v
                };

                if (v <= (uint64_t)std::numeric_limits<u32>::max())
                    return Value{
                        std::in_place_type<u32>,
                        (u32)v
                };
            }
            else {
                const auto v = j.get<int64_t>();

                if (v >= std::numeric_limits<i32>::min() &&
                    v <= std::numeric_limits<i32>::max())
                    return Value{
                        std::in_place_type<i32>,
                        (i32)v
                };
            }

            // Too large for 32-bit integers: fall back to float.
            return Value{
                std::in_place_type<f32>,
                (f32)j.get<double>()
            };
        }

        if (j.is_number_float())
            return Value{
                std::in_place_type<f32>,
                (f32)j.get<double>()
        };

        if (j.is_string())
            return Value{
                std::in_place_type<str>,
                j.get<str>()
        };

        return std::unexpected(
            KeyValuesParseError::UnsupportedType
        );
    }


    // Array of scalars -> std::vector<T>.
    // Fails on mixed bool/string/number arrays or unsupported elements.
    std::expected<Value, KeyValuesParseError>
        KeyValueArray_fromJSON(const json& arr) {
        bool allBool = true;
        bool allStr = true;
        bool allNum = true;

        bool allInt = true;
        bool allFitI32 = true;
        bool allFitU32 = true;

        for (const auto& e : arr) {
            allBool &= e.is_boolean();
            allStr &= e.is_string();
            allNum &= e.is_number();

            if (!e.is_number())
                continue;

            if (!e.is_number_integer()) {
                allInt = false;
                allFitI32 = false;
                allFitU32 = false;
                continue;
            }

            if (e.is_number_unsigned()) {
                const auto v = e.get<uint64_t>();

                allFitI32 &= v <=
                    (uint64_t)std::numeric_limits<i32>::max();

                allFitU32 &= v <=
                    (uint64_t)std::numeric_limits<u32>::max();
            }
            else {
                const auto v = e.get<int64_t>();

                allFitI32 &=
                    v >= std::numeric_limits<i32>::min() &&
                    v <= std::numeric_limits<i32>::max();

                allFitU32 &=
                    v >= 0 &&
                    v <= (int64_t)std::numeric_limits<u32>::max();
            }
        }

        // Type is unknowable; default to float.
        if (arr.empty())
            return Value{
                std::in_place_type<std::vector<f32>>
        };

        if (allBool) {
            std::vector<bool> v;
            v.reserve(arr.size());

            for (const auto& e : arr)
                v.push_back(e.get<bool>());

            return Value{
                std::in_place_type<std::vector<bool>>,
                std::move(v)
            };
        }

        if (allStr) {
            std::vector<str> v;
            v.reserve(arr.size());

            for (const auto& e : arr)
                v.push_back(e.get<str>());

            return Value{
                std::in_place_type<std::vector<str>>,
                std::move(v)
            };
        }

        if (allNum) {
            if (allInt && allFitI32) {
                std::vector<i32> v;
                v.reserve(arr.size());

                for (const auto& e : arr) {
                    if (e.is_number_unsigned())
                        v.push_back((i32)e.get<uint64_t>());
                    else
                        v.push_back((i32)e.get<int64_t>());
                }

                return Value{
                    std::in_place_type<std::vector<i32>>,
                    std::move(v)
                };
            }

            if (allInt && allFitU32) {
                std::vector<u32> v;
                v.reserve(arr.size());

                for (const auto& e : arr) {
                    if (e.is_number_unsigned())
                        v.push_back((u32)e.get<uint64_t>());
                    else
                        v.push_back((u32)e.get<int64_t>());
                }

                return Value{
                    std::in_place_type<std::vector<u32>>,
                    std::move(v)
                };
            }

            std::vector<f32> v;
            v.reserve(arr.size());

            for (const auto& e : arr)
                v.push_back((f32)e.get<double>());

            return Value{
                std::in_place_type<std::vector<f32>>,
                std::move(v)
            };
        }

        return std::unexpected(
            KeyValuesParseError::MixedArrayTypes
        );
    }


    std::expected<void, KeyValuesParseError>
        KeyValues_fromJSON(
            const json& j,
            const str& key,
            KeyValues& out,
            char delimiter
        ) {
        if (j.is_object()) {
            for (auto it = j.begin(); it != j.end(); ++it) {
                const str childKey =
                    key.empty()
                    ? it.key()
                    : key + delimiter + it.key();

                auto result = KeyValues_fromJSON(
                    it.value(),
                    childKey,
                    out,
                    delimiter
                );

                if (!result)
                    return std::unexpected(result.error());
            }

            return {};
        }

        if (j.is_array()) {
            const bool structured = std::any_of(
                j.begin(),
                j.end(),
                [](const json& e) {
                return e.is_object() || e.is_array();
            }
            );

            if (structured) {
                // Arrays containing objects/arrays:
                // key.0, key.1, ...

                size_t i = 0;

                for (const auto& e : j) {
                    const str childKey =
                        key + delimiter + std::to_string(i++);

                    auto result = KeyValues_fromJSON(
                        e,
                        childKey,
                        out,
                        delimiter
                    );

                    if (!result)
                        return std::unexpected(result.error());
                }

                return {};
            }

            auto result = KeyValueArray_fromJSON(j);

            if (!result)
                return std::unexpected(result.error());

            out[key] = std::move(*result);

            return {};
        }

        // Null is intentionally ignored.
        if (j.is_null())
            return {};

        auto result = KeyValueScalar_fromJSON(j);

        if (!result)
            return std::unexpected(result.error());

        out[key] = std::move(*result);

        return {};
    }

}