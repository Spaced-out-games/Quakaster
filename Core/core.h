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


template <template <class...> class Container_t, class TypeList_t>
struct UnpackTypeList;

template <template <class...> class Container_t, class... Ts>
struct UnpackTypeList<Container_t, type_list<Ts...>> {
    using type = Container_t<Ts...>;
};

template <template <class...> class Container_t, class TypeList_t>
using UnpackTypeList_t = typename UnpackTypeList<Container_t, TypeList_t>::type;






template <template <class...> class Container_t, class TypeList_t>
struct UnfoldTypeList;

template <template <class...> class Container_t, class... Ts>
struct UnfoldTypeList<Container_t, type_list<Ts...>> {
    using type = type_list<Container_t<Ts>...>;
};

template <template <class...> class Container_t, class TypeList_t>
using UnfoldTypeList_t = typename UnfoldTypeList<Container_t, TypeList_t>::type;


template <class... Lists>
struct ConcatTypeLists;

template <>
struct ConcatTypeLists<> {
    using type = type_list<>;
};

template <class... Ts>
struct ConcatTypeLists<type_list<Ts...>> {
    using type = type_list<Ts...>;
};

template <class... A, class... B, class... Rest>
struct ConcatTypeLists<type_list<A...>, type_list<B...>, Rest...> {
    using type = typename ConcatTypeLists<
        type_list<A..., B...>, Rest...
    >::type;
};

template <class... Lists>
using ConcatTypeLists_t = typename ConcatTypeLists<Lists...>::type;