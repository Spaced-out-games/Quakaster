/// **************************************** QUAKASTER ENGINE **************************************** 
/// Core/core.h
/// Purpose: Defines core Quakaster macros and types.
/// **************************************************************************************************
#pragma once
#include <source_location>
#include <spdlog/spdlog.h>
#include <cstdint>
#include <cstdlib>
#include <csignal>
#include <string_view>
#include <string>
#include <type_traits>

#define QK_API_VERSION(major, minor, patch) (((u32)(major)) << 22U) | (((u32)(minor)) << 12U) | ((u32)(patch)))

// todo: move these to CMakeLists. Doesn't really matter rn lol
#define QK_VERSION_MAJOR 0
#define QK_VERSION_MINOR 0
#define QK_VERSION_PATCH 0
#define QK_VERSION_DESCRIPTION "Updating netcode framework to a new ECS-based design"


#if QK_DEBUG
    #define QK_BUILD_CONFIGURATION "Debug"
#else
    #define QK_BUILD_CONFIGURATION "Release"
#endif


#define QK_VERSION QK_API_VERSION(QK_VERSION_MAJOR, QK_VERSION_MINOR, QK_VERSION_PATCH) // pre-release

constexpr float QK_EPSILON = 1.0e-5f;

// ---- Shared Library export ----
#ifdef QK_SHARED
#if defined(_MSC_VER)
#ifdef QK_EXPORT
#define QK_API __declspec(dllexport)
#else
#define QK_API __declspec(dllimport)
#endif
#else
#define QK_API __attribute__((visibility("default")))
#endif
#else
#define QK_API
#endif

// ---- Debug break ----
#if defined(_MSC_VER)
#include <intrin.h>
#define QK_DEBUG_BREAK() __debugbreak()
#elif defined(__has_builtin)
#if __has_builtin(__builtin_debugtrap)
#define QK_DEBUG_BREAK() __builtin_debugtrap()
#endif
#endif

#ifndef QK_DEBUG_BREAK
#if defined(__i386__) || defined(__x86_64__)
#define QK_DEBUG_BREAK() __asm__ volatile("int3")
#elif defined(__aarch64__)
#define QK_DEBUG_BREAK() __asm__ volatile("brk #0xf000")
#elif defined(__arm__)
#define QK_DEBUG_BREAK() __asm__ volatile(".inst 0xe7f001f0")
#else
#define QK_DEBUG_BREAK() std::raise(SIGTRAP)
#endif
#endif

// ---- Assert ----
namespace qk {

    inline void assert_failed(
        const char* condition,
        std::string_view message,
        const std::source_location location = std::source_location::current())
    {
        spdlog::critical("QK_ASSERT({}) FAILED:", condition);
        spdlog::critical("Detail: {}", message);
        spdlog::critical("Where: {} @ {} [line {}]",
            location.function_name(), location.file_name(), location.line());
        spdlog::default_logger()->flush();
    }

}

// Define QK_DEBUG in your debug build configs (e.g. via CMake), instead of relying on _DEBUG.
#if(QK_DEBUG)
#define QK_ASSERT(expression, message)                          \
        do {                                                        \
            if (!(expression)) {                                    \
                ::qk::assert_failed(#expression, message);          \
                QK_DEBUG_BREAK();                                   \
            }                                                       \
        } while (0)
#else
#define QK_ASSERT(expression, message) ((void)0)
#endif


#if(QK_DEBUG)
#define QK_SOFT_ASSERT(expression, message)                          \
        do {                                                        \
            if (!(expression)) {                                    \
                ::qk::assert_failed(#expression, message);          \
            }                                                       \
        } while (0)
#else
#define QK_SOFT_ASSERT(expression, message) ((void)0)
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




// Compile-time type sequence.
template<typename... Ts>
struct type_list {};


// Tests whether a type exists in a type_list.
template<typename T, typename List>
struct contains_type;

template<typename T, typename... Ts>
struct contains_type<T, type_list<Ts...>>
    : std::bool_constant<(std::is_same_v<T, Ts> || ...)> {};

template<typename T, typename List>
inline constexpr bool contains_type_v = contains_type<T, List>::value;


// Every type in Required exists in Available.
template<typename Required, typename Available>
struct contains_all_types;

template<typename... Ts, typename... Us>
struct contains_all_types<type_list<Ts...>, type_list<Us...>>
    : std::bool_constant<
    (contains_type<Ts, type_list<Us...>>::value && ...)
    > {};

template<typename Required, typename Available>
inline constexpr bool contains_all_types_v =
contains_all_types<Required, Available>::value;


// At least one type in Required exists in Available.
template<typename Required, typename Available>
struct contains_any_type;

template<typename... Ts, typename... Us>
struct contains_any_type<type_list<Ts...>, type_list<Us...>>
    : std::bool_constant<
    (contains_type<Ts, type_list<Us...>>::value || ...)
    > {};

template<typename Required, typename Available>
inline constexpr bool contains_any_type_v =
contains_any_type<Required, Available>::value;


// Expands a type_list into a variadic template.
template<template<class...> class Container_t, typename TypeList>
struct apply_type_list;

template<template<class...> class Container_t, typename... Ts>
struct apply_type_list<Container_t, type_list<Ts...>> {
    using type = Container_t<Ts...>;
};

template<template<class...> class Container_t, typename TypeList>
using apply_type_list_t =
typename apply_type_list<Container_t, TypeList>::type;


// Applies a template to every type in a type_list.
template<template<class...> class Container_t, typename TypeList>
struct transform_type_list;

template<template<class...> class Container_t, typename... Ts>
struct transform_type_list<Container_t, type_list<Ts...>> {
    using type = type_list<Container_t<Ts>...>;
};

template<template<class...> class Container_t, typename TypeList>
using transform_type_list_t =
typename transform_type_list<Container_t, TypeList>::type;


// Concatenates multiple type_lists.
template<typename... Lists>
struct concat_type_lists;

template<>
struct concat_type_lists<> {
    using type = type_list<>;
};

template<typename... Ts>
struct concat_type_lists<type_list<Ts...>> {
    using type = type_list<Ts...>;
};

template<typename... A, typename... B, typename... Rest>
struct concat_type_lists<type_list<A...>, type_list<B...>, Rest...> {
    using type = typename concat_type_lists<
        type_list<A..., B...>, Rest...
    >::type;
};

template<typename... Lists>
using concat_type_lists_t =
typename concat_type_lists<Lists...>::type;