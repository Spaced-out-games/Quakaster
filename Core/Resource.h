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

        bool flagged() const;

        void flag_for_deletion();
        void acquire();

        void release();
        Resource(const str& ResourceTypeName);
        ~Resource();
    };

    // goal: make the rhi completely explicit, then wrap it at the registry level



}

