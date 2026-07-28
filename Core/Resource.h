/// **************************************** QUAKASTER ENGINE **************************************** 
/// components/Resource.h
/// Purpose: Defines a minimal header for PIMPL-based resource types. 
/// **************************************************************************************************
#pragma once

//#include <type_traits>
#include <utility>
#include "core.h"
#include <atomic>
#include <type_traits>
#include <unordered_map>
#include <spdlog/spdlog.h>


namespace qk
{

     /*
     * NOTE:
     * Resource destruction and object deallocation are fully decoupled.
     *
     * `try_destroy()` releases the underlying resource (e.g. GPU/API object)
     * only when it is safe to do so. The C++ object itself must still be
     * explicitly deallocated afterward. Calling just try_destroy() will cause a memory leak
     *
     * Example:
     *
     * if (pResource->try_destroy())
     *     delete pResource;
     */
    class Resource {

        std::atomic<u32> m_RefCount = 0;
        std::atomic<bool> m_FlaggedForDeletion = false;

        #ifdef _DEBUG 
        str m_Name;
        #endif

    public:

        //virtual void destroy();





        // Sets the label in debug builds
        void label(const str& name);

        // Returns the string value in debug builds, nullptr in release
        const str* label() const;

        u32 refcount() const;
        void refcount_override(u32 new_refcount);

        bool flagged() const;

        void flag_for_deletion();
        void acquire();

        void release();
        Resource(const str& ResourceTypeName);
        ~Resource();
    };

    // Since we need a way for ResourceRegistry to acquire() and release(), we need a generic way for us to take a Resource_t* -> Resource*, but unfortunately, templates are inline and as a consequence
    // the linker will never have the capacity to know how to do the conversion safely. The way around this is simple: For any Resource_t, the following should be defined as free functions 
    // using overloads following the pattern:
    //Resource* Resource_cast(Resource_t* pResource);


    template <class Resource_t>
    using Resource_dtor_pfn_t = void(*)(Resource_t* const& hTarget);

    template <class Resource_t>
    using Resource_free_pfn_t = void(*)(Resource_t*& hTarget);

    // Must uniquely identify a resource within a registry.
    // Generation strategy is intentionally left to the caller.
    //
    // Quakaster commonly uses compile-time string hashes (e.g. "texture"_sh),
    // but any stable, unique 64-bit identifier is valid. No need to force a
    // convention down your throat.
    using Resource_key_t = u64;

    template <class Resource_t, Resource_dtor_pfn_t<Resource_t> Resource_dtor, Resource_free_pfn_t<Resource_t> Resource_free>
    class ResourceRegistry {
        std::unordered_map<Resource_key_t, Resource_t*> m_Resources;

        public:

            void insert(const Resource_key_t& key, Resource_t* pResource);

            bool try_remove(const Resource_key_t& key);

            Resource_t* peek(const Resource_key_t& key);

            Resource_t* acquire(const Resource_key_t& key);

            bool try_release(const Resource_key_t& key);

            void collect_pending();


            void destroy_everything();

            ~ResourceRegistry();

    };

    


}

#include "Resource.inl"