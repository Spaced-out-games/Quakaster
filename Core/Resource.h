/// **************************************** QUAKASTER ENGINE **************************************** 
/// components/Resource.h
/// Purpose: Defines a minimal header for PIMPL-based resource types. 
/// **************************************************************************************************
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
        // NOTICE: When using initializer lists on derivative construction, the compiler would assume we were trying to construct Resource metadata, so it would forward the args here.
        // This constructor is here to make Resource a non-aggregate, forcing the compiler to give the arguments to the derivative.
        Resource() = default;

        
        bool ready_for_deletion() const {
            bool result = references == 0;
            //result = result && state == Retired;
            return result;
        
        }

        // Whether or not the resource is valid
        bool valid() const { return state == Active; }
        
        virtual bool destroy() {
            return true;
        }

        // default destructor, for now.
        virtual ~Resource() {
            assert(references == 0);
            destroy();
        }


    };

    // This probably needs cleaned up in order to make sure pimpl's stay valid, and that hash collisions don't occur
    template<typename Resource_t>
    class ResourceRegistry
    {
        static_assert(
            std::is_base_of_v<Resource, Resource_t>,
            "Resource_t must derive from qk::gfx::rhi::Resource"
        );

    public:

        ResourceRegistry() {
            // This is here to prevent reallocations and subsequent invalidations of impl*s. 
            m_Resources.reserve(1024);
        }

        ~ResourceRegistry()
        {
            for (const auto& resource : m_Resources) {
                auto* pResource = resource.second;
                delete pResource;
                std::cout << pResource;
            }
        }

        // Insert a resource
        Resource_t* insert(
            str_view label,
            Resource_t* resource);

        // destroy a resource
        bool destroy(
            Resource_t*& resource);

        // Validates an impl*
        bool valid(
            const Resource_t* resource) const;

        Resource_t* find(
            str_view label);

        // Reverse lookup
        const Resource_t* find(
            str_view label) const;

        // Reverse lookup
        Resource_t* find(
            u64 id);

        // Reverse lookup
        const Resource_t* find(
            u64 id) const;

        // Increments the refcount of the resource by one
        bool acquire(
            const Resource_t* resource);

        // Decrements the refcount of the resource by one
        bool release(
            const Resource_t* resource);

        // Gets the refcount of the resource
        int refcount(
            const Resource_t* resource) const;

        // gets the label of the resource
        str_view label(
            const Resource_t* resource) const;

        // gets the resource identity hash code
        u64 id(
            const Resource_t* resource) const;

        // Invalidates the resources so garbage collection can pick up on it
        void clear();

        size_t size() const;

    private:

        std::unordered_map<u64, Resource_t*> m_Resources;
    };

}

#include "Resource.inl"