#pragma once
#include <cstdint>
#include <utility>

namespace qk {

    using handle_t = uintptr_t;
    constexpr handle_t NULL_HANDLE = 0;

    template <class T>
    inline constexpr bool dependent_false_v = false;

    // -----------------------------------------------------------------------
    //  Handle_dtor — declared here, specialised in engine .cpp files.
    //  NO ifdef guard: ~Handle() is inline and must compile in all TUs.
    //  If you forget to specialise for a tag, you get a linker error.
    // -----------------------------------------------------------------------
    template <class tag_t>
    void Handle_dtor(handle_t handle) noexcept; // intentionally no definition

    // -----------------------------------------------------------------------

    template <class tag_t>
    class Handle {
    public:

        #ifdef QK_BUILD_ENGINE
        template <class U, class T>
        friend Handle<U> Handle_ctor(const T& internal_handle);

        template <class U, class T>
        friend T Handle_cast(const Handle<U>& hdl);

        template <class T>
        friend void Handle_clear(Handle<T>& hdl);
        #endif

        Handle() = default;

        Handle(const Handle&) = delete;
        Handle& operator=(const Handle&) = delete;

        Handle(Handle&& other) noexcept
            : handle(std::exchange(other.handle, NULL_HANDLE)) {}

        Handle& operator=(Handle&& other) noexcept {
            if (this != &other) {
                // Destroy what we currently own before overwriting
                if (valid()) Handle_dtor<tag_t>(handle);
                handle = std::exchange(other.handle, NULL_HANDLE);
            }
            return *this;
        }

        ~Handle() noexcept {
            if (valid()) Handle_dtor<tag_t>(handle);
        }

        bool valid() const noexcept { return handle != NULL_HANDLE; }

    private:
        handle_t handle = NULL_HANDLE;
    };


    #ifdef QK_BUILD_ENGINE

    template <class tag_t, class T>
    Handle<tag_t> Handle_ctor(const T&) {
        static_assert(dependent_false_v<T>, "Handle_ctor not specialized for this tag");
    }

    template <class tag_t, class T>
    T Handle_cast(const Handle<tag_t>&) {
        static_assert(dependent_false_v<T>, "Handle_cast not specialized for this tag");
    }

    template <class T>
    void Handle_clear(Handle<T>& hdl) {
        hdl.handle = NULL_HANDLE;
    }

    #endif

} // namespace qk