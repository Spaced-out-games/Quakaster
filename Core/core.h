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

constexpr float QK_EPSILON = 0.0e5;

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


template<typename... Ts>
struct type_list {};

template<typename T, typename List>
struct contains_T;

template<typename T, typename... Ts>
struct contains_T<T, type_list<Ts...>>
    : std::disjunction<std::is_same<T, Ts>...> {};

template<typename T, typename List>
inline constexpr bool is_type_in_list_v = contains_T<T, List>::value;


// Every type in the first list exists in the second list.
template<typename Required, typename Available>
struct all_of_Ts;

template<typename... Ts, typename... Us>
struct all_of_Ts<type_list<Ts...>, type_list<Us...>>
    : std::conjunction<contains_T<Ts, type_list<Us...>>...> {};

template<typename Required, typename Available>
inline constexpr bool all_of_Ts_v = all_of_Ts<Required, Available>::value;


// At least one type in the first list exists in the second list.
template<typename Required, typename Available>
struct any_of_Ts;

template<typename... Ts, typename... Us>
struct any_of_Ts<type_list<Ts...>, type_list<Us...>>
    : std::disjunction<contains_T<Ts, type_list<Us...>>...> {};

template<typename Required, typename Available>
inline constexpr bool any_of_Ts_v = any_of_Ts<Required, Available>::value;