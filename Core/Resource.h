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

#ifdef _DEBUG
#include <spdlog/spdlog.h>
#endif

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

        virtual void destroy() {};

        // Whether or not this resource is ready to be deleted. 
        bool can_destroy() {
            return m_RefCount == 0 && m_FlaggedForDeletion;
        }

        // Try to destroy the resource
        bool try_destroy() {
            if (!can_destroy()) return false;

            destroy();
            return true;
        }

        // Sets the label in debug builds
        void set_label(const str& label) {
            #ifdef _DEBUG
            m_Label = label;
            m_Hash = (u64)hash_fnv1a(label);
            #endif
        }

        // Returns the string value in debug builds, nullptr in release
        const str* label() const {
            #ifdef _DEBUG
            return &m_Label;
            #else
            return nullptr;
            #endif

        }

        // Returns the hash code in debug builds, nullptr in release
        const u64* hash_code() const {
            #ifdef _DEBUG
            return &m_Hash;
            #else
            return nullptr;
            #endif

        }

        u32 refcount() const {
            return m_RefCount;
        }

        bool flagged() const {
            return m_FlaggedForDeletion;
        }

        void flag_for_deletion() {
            m_FlaggedForDeletion = true;
        }

        ~Resource() {
            #ifdef _DEBUG
            spdlog::info("Destroyed resource `{}` at ", m_Label);
            #endif
        }
    };





}

