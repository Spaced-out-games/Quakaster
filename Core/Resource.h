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


    class Resource {
        #ifdef _DEBUG 
        str m_Label;
        #endif
        // we should keep the hash in release
        u64 m_Hash;
        std::atomic<u32> m_RefCount = 0;
        std::atomic<bool> m_FlaggedForDeletion = false;

    public:

        virtual void destroy();

        // Whether or not this resource is ready to be deleted. 
        bool can_destroy();

        // Try to destroy the resource
        bool try_destroy();

        // Sets the label in debug builds
        void set_label(const str& label);

        // Returns the string value in debug builds, nullptr in release
        const str* label() const;

        // Returns the hash code in debug builds, nullptr in release
        const u64* hash_code();

        u32 refcount() const;

        bool flagged() const;

        void flag_for_deletion();
        void acquire();

        void release();

        ~Resource();
    };


    template <class Resource_t>
    class ResourceManager {
        static_assert(std::is_base_of_v<Resource, Resource_t>);



        std::unordered_map<u64, Resource*> m_Resources;

    public:

        void insert(u64 id, Resource_t* pResource);

        Resource_t* get(u64 id);

        bool can_destroy(u64 id);

    };



}

