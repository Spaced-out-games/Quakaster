#pragma once
#include <cstdint>
#include <utility>

namespace qk {

    template<class Tag>
    struct handle_traits; // must be forward declared FIRST

    template<class Tag>
    class Handle;


    template<class Tag>
    class Ref {
        const Handle<Tag>& h;

    public:
        Ref(const Handle<Tag>& handle) : h(handle) {}

        // implicit access to underlying handle
        operator const Handle<Tag>& () const noexcept {
            return h;
        }

        const Handle<Tag>* operator->() const noexcept {
            return &h;
        }

        const Handle<Tag>& get() const noexcept {
            return h;
        }
    };




    template<class Tag>
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


        Ref<Tag> ref() const noexcept {
            return Ref<Tag>(*this);
        }

    private:
        template<class T>
        friend void set(Handle<T>&, typename handle_traits<T>::value_type);
    };

    template<class Tag>
    typename handle_traits<Tag>::value_type
        get(const Handle<Tag>& h)
    {
        return (typename handle_traits<Tag>::value_type)(h.value());
    }

    template<class Tag>
    void set(Handle<Tag>& h,
        typename handle_traits<Tag>::value_type v)
    {
        h.m_Handle = (uintptr_t)(v);
    }

} // namespace qk