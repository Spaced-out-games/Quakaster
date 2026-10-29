#pragma once
#include <entt/entity/entity.hpp>
#include <core.h>

namespace qk {



    struct UURH {
        static constexpr u32 NULL_ENTRY = UINT32_MAX;
        static constexpr u16 NULL_DRIVE = UINT16_MAX;

        u32 EntryID = NULL_ENTRY;
        u16 DriveID = NULL_DRIVE;

        template <class T>
        inline T to() const {
            static_assert(!sizeof(T), "Unsupported UURH conversion");
        }

        template <class T>
        inline static constexpr u16 drive_id = NULL_DRIVE;
    };

}