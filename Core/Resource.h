/// **************************************** QUAKASTER ENGINE **************************************** 
/// components/Resource.h
/// Purpose: Defines a minimal header for PIMPL-based resource types. 
/// **************************************************************************************************
#pragma once

//#include <type_traits>
#include <utility>
#include "core.h"
#include <atomic>



namespace qk
{


    class Resource {
        #ifdef _DEBUG
        str m_Label;
        u64 m_Hash;
        #endif
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





}

