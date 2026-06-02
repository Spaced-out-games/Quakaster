#pragma once

#include <unordered_map>
#include <type_traits>
#include <utility>

#include <core.h>
#include <fnv1a.h>

namespace qk
{
    struct Resource
    {
        u64 id = 0;
        u32 refs = 0;
        str label;
    };

    template<typename Resource_t>
    class ResourceRegistry
    {
        static_assert(
            std::is_base_of_v<Resource, Resource_t>,
            "Resource_t must derive from qk::gfx::rhi::Resource");

    public:

        ResourceRegistry() {
            m_Resources.reserve(1024); // just see if this fixes it
        }

        Resource_t* insert(
            str_view label,
            Resource_t&& resource);

        bool destroy(
            Resource_t* resource);

        bool valid(
            const Resource_t* resource) const;

        Resource_t* find(
            str_view label);

        const Resource_t* find(
            str_view label) const;

        Resource_t* find(
            u64 id);

        const Resource_t* find(
            u64 id) const;

        bool acquire(
            const Resource_t* resource);

        bool release(
            const Resource_t* resource);

        int refcount(
            const Resource_t* resource) const;

        str_view label(
            const Resource_t* resource) const;

        u64 id(
            const Resource_t* resource) const;

        void clear();

        size_t size() const;

    private:

        std::unordered_map<u64, Resource_t> m_Resources;
    };

}

#include "Resource.inl"