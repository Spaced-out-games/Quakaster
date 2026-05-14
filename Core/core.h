// Core/core.h
#include <cassert>



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


