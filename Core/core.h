/// **************************************** QUAKASTER ENGINE **************************************** 
/// Core/core.h
/// Purpose: Defines core Quakaster macros and types.
/// **************************************************************************************************
#include <cassert>
#include <stdint.h>


#define QK_API_VERSION(variant, major, minor, patch) \
    ((((uint32_t)(variant)) << 29U) | (((uint32_t)(major)) << 22U) | (((uint32_t)(minor)) << 12U) | ((uint32_t)(patch)))
// pre-release
#define QK_VERSION QK_API_VERSION(0,0,0)

#define QK_ASSERT(exp) assert(exp)


#pragma once
#ifdef QK_SHARED
#ifdef QK_EXPORT
#define QK_API __declspec(dllexport)
#else
#define QK_API __declspec(dllimport)
#endif
#else
#define QK_API  // static no decoration needed
#endif

// QK_Debug.h
#pragma once

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


using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;



using f32 = float;
static_assert((sizeof(float) == 4));

using f64 = double;
static_assert((sizeof(double) == 8));

