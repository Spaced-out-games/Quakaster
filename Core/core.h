/// **************************************** QUAKASTER ENGINE **************************************** 
/// Core/core.h
/// Purpose: Defines core Quakaster macros and types.
/// **************************************************************************************************
#pragma once
#include <cassert>
#include <stdint.h>
#include <string_view>
#include <string>

#define QK_API_VERSION(variant, major, minor, patch) \
    ((((u32)(variant)) << 29U) | (((u32)(major)) << 22U) | (((u32)(minor)) << 12U) | ((u32)(patch)))
// pre-release
#define QK_VERSION QK_API_VERSION(0,0,0)

#define QK_ASSERT(exp) assert(exp)

constexpr float QK_EPSILON = 0.0e6;

#pragma once
#ifdef QK_SHARED
#ifdef QK_EXPORT
#define QK_API __declspec(dllexport)
#else
#define QK_API __declspec(dllimport)
#endif
#else
#define QK_API
#endif


#if defined(_MSC_VER)
// MSVC
#define QK_DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#include <signal.h>
#define QK_DEBUG_BREAK() raise(SIGTRAP)
#else
#include <cstdlib>
#define QK_DEBUG_BREAK() std::abort() // fallback
#endif


using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using f32 = float;
static_assert((sizeof(float) == 4));
using f64 = double;
static_assert((sizeof(double) == 8));




using str = std::string;
using str_view = std::string_view;

// Loads from the OS' global symbol table
typedef void* (*SymbolLoader)(const char* name);

// Generic Window resource handle
using HWindow = void*;