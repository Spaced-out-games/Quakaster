#pragma once
#include <stdint.h>
#include <string_view>
#define FNV1A_OFFSET_BASIS 14695981039346656037
#define FNV1A_PRIME 1099511628211



namespace qk {

	using fnv1a_hash_t = uint64_t;



    template <size_t N>
    constexpr fnv1a_hash_t hash_fnv1a(const char(&value)[N]) {
        fnv1a_hash_t hash = FNV1A_OFFSET_BASIS;

        for (size_t i = 0; i < N - 1; ++i) {
            hash ^= static_cast<uint8_t>(value[i]);
            hash *= FNV1A_PRIME;
        }

        return hash;
    }


    struct hashed_string {
        uint64_t hash;
        std::string_view str;

        constexpr hashed_string(uint64_t h, std::string_view s = {})
            : hash(h), str(s) {}

        template <size_t N>
        constexpr hashed_string(const char(&value)[N])
            : hash(hash_fnv1a(value)),
            str(value, N - 1) {}

        constexpr operator uint64_t() const {
            return hash;
        }

        constexpr bool operator==(const hashed_string& other) const {
            return hash == other.hash;
        }
    };


}