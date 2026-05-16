/// **************************************** QUAKASTER ENGINE **************************************** 
/// Core/Result.h
/// Purpose: Defines an opaque Resource type that smuggles Resources past encapsulation borders.
///          Also defines an opaque reference to these Resources
/// **************************************************************************************************
#pragma once
#include <cstdint>
#include <utility>
#define QK_NULL_RESOURCE ((uintptr_t)UINTPTR_MAX)

namespace qk {

    template<class tag_t>
    struct ResourceTraits;

    template<class tag_t>
    class Resource;


    template<class tag_t>
    class Ref {
        const Resource<tag_t>& m_Resource;

    public:
        Ref(const Resource<tag_t>& Resource) : m_Resource(Resource) {}

        // implicit access to underlying Resource
        operator const Resource<tag_t>& () const noexcept {
            return m_Resource;
        }

        const Resource<tag_t>* operator->() const noexcept {
            return &m_Resource;
        }

        const Resource<tag_t>& get() const noexcept {
            return m_Resource;
        }
    };




    template<class tag_t>
    class Resource {
        uintptr_t m_Resource;

    public:
        constexpr Resource() noexcept: m_Resource(QK_NULL_RESOURCE) {}

        constexpr Resource(std::nullptr_t) noexcept: m_Resource(QK_NULL_RESOURCE) {}
        constexpr explicit Resource(uintptr_t hValue) noexcept: m_Resource(hValue) {}

        bool valid() const noexcept {
            return m_Resource != QK_NULL_RESOURCE;
        }

        uintptr_t value() const noexcept {
            return m_Resource;
        }

        // PSA: It is entirely possible for you to call foo() -> Resource<T>.ref() -> Ref<T> and it end up
        // causing a forced memory leak, since Resource destructors will (or at least SHOULD) take a non-const&,
        // meaning it is damn impossible to call the destructor with the ref without causing UB since it would
        // discard the non - const&. It *shouldn't* be capable of anything other than a forced memory leak.


        Ref<tag_t> ref() const noexcept {
            return Ref<tag_t>(*this);
        }

    private:
        template<class T>
        friend void set(Resource<T>&, typename ResourceTraits<T>::value_type);
    };

    template<class tag_t>
    typename ResourceTraits<tag_t>::value_type
        get(const Resource<tag_t>& h)
    {
        return (typename ResourceTraits<tag_t>::value_type)(h.value());
    }

    template<class tag_t>
    void set(Resource<tag_t>& h,
        typename ResourceTraits<tag_t>::value_type v)
    {
        h.m_Resource = (uintptr_t)(v);
    }

} // namespace qk