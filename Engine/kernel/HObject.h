#pragma once
#include <entt/entity/entity.hpp>
#include <core.h>

namespace qk {

    using DriveID_t = u16;


    // Quakaster's Universally Unique Object Identifier type.
    template <class ...ID_Ts>
    struct HObject {

        static constexpr DriveID_t UnmountedDrive = UINT16_MAX;

        entt::entity EntryID = entt::null;
        DriveID_t    DriveID = UnmountedDrive;

        constexpr HObject() noexcept = default;

        template <class ID_T>
            requires (std::is_same_v<ID_T, ID_Ts> || ...)
        static constexpr DriveID_t to_drive_id(ID_T id) noexcept
        {
            return static_cast<DriveID_t>(entt::to_integral(id));
        }

        template <class ID_T>
            requires (std::is_same_v<ID_T, ID_Ts> || ...)
        constexpr HObject(ID_T id, DriveID_t drive = UnmountedDrive) noexcept
            : EntryID(static_cast<entt::entity>(entt::to_integral(id)))
            , DriveID(drive)
        {}

        constexpr bool is_null() const noexcept
        {
            return EntryID == entt::null;
        }

        constexpr bool is_mounted() const noexcept
        {
            return DriveID != UnmountedDrive;
        }

        template <class ID_T>
        constexpr ID_T resolve() const noexcept
        {
            /* as before */
        }
    };

}