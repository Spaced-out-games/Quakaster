#pragma once
#include <cstdint>
#include <utility>

namespace qk {

    template<class tag_t>
    struct handle_traits; // must be forward declared FIRST

    template<class tag_t>
    class Handle;


    template<class tag_t>
    class Ref {
        const Handle<tag_t>& m_Handle;

    public:
        Ref(const Handle<tag_t>& handle) : m_Handle(handle) {}

        // implicit access to underlying handle
        operator const Handle<tag_t>& () const noexcept {
            return m_Handle;
        }

        const Handle<tag_t>* operator->() const noexcept {
            return &m_Handle;
        }

        const Handle<tag_t>& get() const noexcept {
            return m_Handle;
        }
    };




    template<class tag_t>
    class Handle {
        uintptr_t m_Handle = 0;

    public:
        Handle() = default;
        Handle(std::nullptr_t) : m_Handle(0) {}

        bool valid() const noexcept {
            return m_Handle != 0;
        }

        uintptr_t value() const noexcept {
            return m_Handle;
        }

        // PSA: It is entirely possible for you to call foo() -> Handle<T>.ref() -> Ref<T> and it end up
        // causing a forced memory leak, since handle destructors will (or at least SHOULD) take a non-const&,
        // meaning it is damn near impossible to call the destructor with the ref since it would discard the
        // non - const&. It *shouldn't* be capable of anything other than a forced memory leak.


        Ref<tag_t> ref() const noexcept {
            return Ref<tag_t>(*this);
        }

    private:
        template<class T>
        friend void set(Handle<T>&, typename handle_traits<T>::value_type);
    };

    template<class tag_t>
    typename handle_traits<tag_t>::value_type
        get(const Handle<tag_t>& h)
    {
        return (typename handle_traits<tag_t>::value_type)(h.value());
    }

    template<class tag_t>
    void set(Handle<tag_t>& h,
        typename handle_traits<tag_t>::value_type v)
    {
        h.m_Handle = (uintptr_t)(v);
    }

} // namespace qk