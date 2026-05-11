#pragma once
#include "core.h"
#include <cstdint>
#include <utility>
#define NULL_HANDLE 0

namespace qk {
    template <class tag_t>
    class Handle;


    template <class tag_t>
    using Ref = const Handle<tag_t>;

    template <class T>
    inline constexpr bool dependent_false_v = false;

    using handle_t = uintptr_t;

    template <class tag_t>
    class Handle {
        handle_t m_Handle = NULL_HANDLE;

    public:
        template <class tag_t, class T>
        friend T get(const Handle<tag_t>&);

        template <class tag_t, class T>
        friend void set(Handle<tag_t>& hdl, T value);

        Handle() = default;
        Handle(std::nullptr_t nil) {
            m_Handle = NULL_HANDLE;
        }

        Handle(explicit uintptr_t handle) : m_Handle(handle) {}


        Handle(const Handle&) = delete;
        Handle& operator=(const Handle&) = delete;

        Handle(Handle&& other) noexcept
            : m_Handle(std::exchange(other.m_Handle, NULL_HANDLE)) {}

        Handle& operator=(Handle&& other) noexcept {
            if (this != &other) {

                m_Handle = std::exchange(other.m_Handle, NULL_HANDLE);
            }
            return *this;
        }

        ~Handle() = default;

        bool valid() const noexcept { return m_Handle != NULL_HANDLE; }

        // To prevent double-frees, we disable copy construction, but allow
        // views.
        Ref<tag_t> copy() const noexcept { return Ref<tag_t>(m_Handle); }

        uintptr_t value() const noexcept { return m_Handle; }

    private:
    };


    


    template <class tag_t, class T>
    T get(const Handle<tag_t>&) {
        //QK_DEBUG_BREAK();
        static_assert(dependent_false_v<T>, "Handle_get not specialized for this tag");
    }

    template <class tag_t, class T>
    void set(Handle<tag_t>& hdl, T value) {
        //QK_DEBUG_BREAK();
        static_assert(dependent_false_v<T>, "Handle_set not specialized for this tag");
    }



} // namespace qk