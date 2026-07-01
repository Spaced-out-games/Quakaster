/// **************************************** QUAKASTER ENGINE **************************************** 
/// Core/fnv1a.h
/// Purpose: Defines an engine-native compile-time string hash and related utilities
/// **************************************************************************************************
#pragma once
#include <stdint.h>
#include <string_view>
#define FNV1A_OFFSET_BASIS 14695981039346656037
#define FNV1A_PRIME 1099511628211



namespace qk {

	using fnv1a_hash_t = uint64_t;



    constexpr fnv1a_hash_t hash_fnv1a(std::string_view value) {
        fnv1a_hash_t hash = FNV1A_OFFSET_BASIS;

        for (char c : value) {
            hash ^= (uint8_t)(c);
            hash *= FNV1A_PRIME;
        }

        return hash;
    }




    // Stores a fnv1a string hash.
    struct string_hash {
        // Might want to add the string contents as well sometime
        uint64_t hash;

        constexpr string_hash(const std::string_view& value)
            : hash(hash_fnv1a(value))
        {}

        constexpr operator uint64_t() const {
            return hash;
        }


        constexpr bool operator==(const string_hash& other) const {
            return hash == other.hash;
        }

        

    };

    [[nodiscard]] constexpr string_hash operator"" _sh(const char* str, std::size_t) noexcept {
        return string_hash{ str };
    }


}