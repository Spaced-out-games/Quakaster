#pragma once
#include <kernel/UURH.h>
#include <KeyValues.h>
#include <kernel/VDrive.h>
#include <kernel/EntityLoader.h>


namespace qk {

    template <>
    inline constexpr u64 UURH::drive_id<entt::entity> = 0;

    template <>
    inline entt::entity UURH::to<entt::entity>() const {
        // catches programmer error in debug
        QK_ASSERT(DriveID == drive_id<entt::entity>, "Attempted unsafe cast of qk::UURH to entt::entity!");
        if (DriveID != drive_id<entt::entity>)
            return entt::null;

        return static_cast<entt::entity>(EntryID);
    }





	
	struct World: public VDrive {
            entt::registry m_Registry;
            EntityLoader m_Loader;
        public:
        
        virtual ~World();

        World(KernelPermissions permissions) : VDrive(permissions) {}

        virtual void execute_loader(qk::UURH target, const KeyValues& args) override;


        // CRUD
        virtual std::expected<qk::UURH, VDriveError> create(const KeyValues&) override;
        virtual std::expected<KeyValues, VDriveError> read(qk::UURH, const KeyValues& request) const override;
        virtual std::expected<void, VDriveError>  update(qk::UURH, const KeyValues& request) override;
        virtual std::expected<bool, VDriveError> destroy(qk::UURH, const KeyValues&) override;

        // validation
        virtual std::expected<bool, VDriveError> valid(qk::UURH) const override;

        // placeholder...consider a bytecode?
        using Query = u32;

        // look up entity by its CLabel
        virtual std::expected<qk::UURH, VDriveError> lookup(str_view label) const override;
        virtual std::expected<std::vector<qk::UURH>, VDriveError> find(const Query&) const override;

        virtual str to_string(qk::UURH target, EntryDisplayFlags flags) override;


        virtual void list(qk::UURH target, const std::vector<str>& keys, EntryDisplayFlags flags) const override;
	};

}
#include "World.inl"