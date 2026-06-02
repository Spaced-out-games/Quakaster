#pragma once

#include <unordered_map>
#include <type_traits>
#include <utility>

#include <core.h>
#include <fnv1a.h>
#include <iostream>
namespace qk
{

    enum ResourceState: u8 {
        Active,
        Retired,
        Destroyed
    };

    struct Resource
    {
        str label;
        u64 hash_code = 0;
        u32 references = 0;
        ResourceState state = Destroyed;
        // NOTICE: Whwn using initializer lists on derivative construction, the compiler would assume we were trying to construct Resource metadata, so it would forward the args here.
        // This constructor is here to make Resource a non-aggregate, forcing the compiler to give the arguments to the derivative.
        Resource() = default;

        
        bool ready_for_deletion() const {
            bool result = references == 0;
            //result = result && state == Retired;
            return result;
        
        }

        // Whether 
        bool valid() const { return state == Active; }
        
        virtual bool destroy() {
            return true;
        }

        // default destructor, for now.
        virtual ~Resource() {
            assert(references == 0);
            destroy();
        }

        // 

    };

    template<typename Resource_t>
    class ResourceRegistry
    {
        static_assert(
            std::is_base_of_v<Resource, Resource_t>,
            "Resource_t must derive from qk::gfx::rhi::Resource"
        );

    public:

        ResourceRegistry() {
            m_Resources.reserve(1024);
        }

        ~ResourceRegistry()
        {
            for (const auto& resource : m_Resources) {
                auto* pResource = resource.second;
                delete pResource;
                std::cout << pResource; // just for debugging
            }
        }

        Resource_t* insert(
            str_view label,
            Resource_t* resource);

        bool destroy(
            Resource_t*& resource);

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

        std::unordered_map<u64, Resource_t*> m_Resources;
    };

}

#include "Resource.inl"