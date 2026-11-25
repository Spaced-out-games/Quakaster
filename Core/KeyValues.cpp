#include "KeyValues.h"

#include <cmath>
#include <cstdint>
#include <limits>
#include <string>
#include <type_traits>

namespace qk {

    namespace {

        enum class JSONType : u8 {
            String,
            Number,
            Boolean,
            Other
        };

        JSONType GetJSONType(const json& j) {
            if (j.is_string())
                return JSONType::String;

            if (j.is_number())
                return JSONType::Number;

            if (j.is_boolean())
                return JSONType::Boolean;

            return JSONType::Other;
        }


        template <class T>
        struct IsVector : std::false_type {};

        template <class T, class Allocator>
        struct IsVector<std::vector<T, Allocator>> : std::true_type {
            using value_type = T;
        };


        template <class T>
        constexpr bool IsSupportedScalar =
            std::same_as<T, str> ||
            std::same_as<T, u32> ||
            std::same_as<T, f32> ||
            std::same_as<T, bool>;


        // Attempts one specific scalar conversion.
        template <class T>
        std::expected<T, KeyValuesParseError>
            KeyValue_fromJSON_scalar(const json& j) {

            if constexpr (std::same_as<T, str>) {
                if (!j.is_string())
                    return std::unexpected(KeyValuesParseError::UnsupportedType);

                return j.get<str>();
            }

            else if constexpr (std::same_as<T, u32>) {
                if (!j.is_number_integer())
                    return std::unexpected(KeyValuesParseError::UnsupportedType);

                if (j.is_number_unsigned()) {
                    const auto value = j.get<std::uint64_t>();

                    if (value > std::numeric_limits<u32>::max())
                        return std::unexpected(KeyValuesParseError::UnsupportedType);

                    return static_cast<u32>(value);
                }

                const auto value = j.get<std::int64_t>();

                if (value < 0 ||
                    value > std::numeric_limits<u32>::max()) {
                    return std::unexpected(KeyValuesParseError::UnsupportedType);
                }

                return static_cast<u32>(value);
            }

            else if constexpr (std::same_as<T, f32>) {
                if (!j.is_number())
                    return std::unexpected(KeyValuesParseError::UnsupportedType);

                const double value = j.get<double>();

                if (!std::isfinite(value) ||
                    value > std::numeric_limits<f32>::max() ||
                    value < -std::numeric_limits<f32>::max()) {
                    return std::unexpected(KeyValuesParseError::UnsupportedType);
                }

                return static_cast<f32>(value);
            }

            else if constexpr (std::same_as<T, bool>) {
                if (!j.is_boolean())
                    return std::unexpected(KeyValuesParseError::UnsupportedType);

                return j.get<bool>();
            }

            else {
                return std::unexpected(KeyValuesParseError::UnsupportedType);
            }
        }


        // Attempts one specific vector conversion.
        template <class T>
        std::expected<T, KeyValuesParseError>
            KeyValue_fromJSON_vector(const json& j) {

            using Element_t = typename IsVector<T>::value_type;

            if (!j.is_array() || j.empty())
                return std::unexpected(KeyValuesParseError::UnsupportedType);

            T result;
            result.reserve(j.size());

            const JSONType expectedType = GetJSONType(j.front());

            for (const auto& element : j) {

                // All elements must have the same JSON category.
                if (GetJSONType(element) != expectedType)
                    return std::unexpected(KeyValuesParseError::MixedArrayTypes);

                auto value = KeyValue_fromJSON_scalar<Element_t>(element);

                if (!value)
                    return std::unexpected(value.error());

                result.emplace_back(std::move(*value));
            }

            return result;
        }

    } // anonymous namespace


    // ------------------------------------------------------------
    // Concrete conversion
    // ------------------------------------------------------------

    template <class T>
    std::expected<T, KeyValuesParseError>
        KeyValue_fromJSON_as(const json& j) {

        if constexpr (IsVector<T>::value) {
            return KeyValue_fromJSON_vector<T>(j);
        }
        else if constexpr (IsSupportedScalar<T>) {
            return KeyValue_fromJSON_scalar<T>(j);
        }
        else {
            return std::unexpected(KeyValuesParseError::UnsupportedType);
        }
    }


    // ------------------------------------------------------------
    // Variant conversion
    // ------------------------------------------------------------

    namespace {

        template <size_t Index = 0>
        std::expected<KeyValue, KeyValuesParseError>
            KeyValue_fromJSON_try(const json& j) {

            if constexpr (Index < std::variant_size_v<KeyValueVariant_t>) {

                using Alternative_t =
                    std::variant_alternative_t<Index, KeyValueVariant_t>;

                auto result = KeyValue_fromJSON_as<Alternative_t>(j);

                if (result) {
                    return KeyValue{
                        std::in_place_type<Alternative_t>,
                        std::move(*result)
                    };
                }

                // A mixed array is a more specific error than an
                // unsupported conversion.
                if (result.error() == KeyValuesParseError::MixedArrayTypes)
                    return std::unexpected(result.error());

                return KeyValue_fromJSON_try<Index + 1>(j);
            }
            else {
                return std::unexpected(KeyValuesParseError::UnsupportedType);
            }
        }

    } // anonymous namespace


    std::expected<KeyValue, KeyValuesParseError>
        KeyValue_fromJSON(const json& j) {
        return KeyValue_fromJSON_try(j);
    }

}