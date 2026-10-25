#pragma once
#include <kernel/HObject.h>
#include <kernel/KeyValues.h>
#include <kernel/VDrive.h>


namespace qk {

	template <class ...ID_Ts>
	struct World: public VDrive<ID_Ts...> {
        
        virtual ~World();

        World(KernelPermissions permissions) : VDrive<ID_Ts...>(permissions) {}

        // CRUD
        virtual std::expected<qk::HObject<ID_Ts...>, VDriveError> create(const KeyValues&) override;
        virtual std::expected<KeyValues, VDriveError> read(qk::HObject<ID_Ts...>, const KeyValues& request) const override;
        virtual std::expected<void, VDriveError>  update(qk::HObject<ID_Ts...>, const KeyValues& request) override;
        virtual std::expected<bool, VDriveError> destroy(qk::HObject<ID_Ts...>, const KeyValues & = {}) override;

        // validation
        virtual std::expected<bool, VDriveError> valid(qk::HObject<ID_Ts...>) const override;

        // placeholder...consider a bytecode?
        using Query = u32;

        // look up entity by its CLabel
        virtual std::expected<qk::HObject<ID_Ts...>, VDriveError> lookup(str_view label) const override;
        virtual std::expected<std::vector<qk::HObject<ID_Ts...>>, VDriveError> find(const Query&) const override;

        virtual str to_string(qk::HObject<ID_Ts...> target, EntryDisplayFlags flags) override;


        virtual void list(qk::HObject<ID_Ts...> target, const std::vector<str>& keys, EntryDisplayFlags flags) const override;
	};

}
#include "World.inl"