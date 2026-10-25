#pragma once
#include "KeyValues.h"
#include "Kernel.h"
#include "HObject.h"
#include <expected>
#include <optional>

namespace qk {



    enum class EntryDisplayFlags : std::uint16_t {
        NONE = 0,
        LONG = 1 << 0,   // ls -l: one entry per line with details
        ALL = 1 << 1,   // ls -a: include hidden entries (unlabeled, internal components)
        PATH = 1 << 2,   // print full path (world:/door_a/cdoor.locked), not just the name
        ID = 1 << 3,   // include the qk::HObject (#3:12v0)
        TYPE = 1 << 4,   // append a type marker (dir/, component, field)
        VALUES = 1 << 5,   // show field values inline (cat-style)
        PERMS = 1 << 6,   // show the effective Permissions mask (ls -l style)
        RECURSIVE = 1 << 7,   // descend into contained entries (-r)
        SORTED = 1 << 8,   // stable sort before printing
        RAW = 1 << 9,   // machine-readable: paths only, one per line, no decoration
    };

    enum class EVDriveErrorCode_t : u8 {
        STATUS_OKAY = 0
    };

    struct VDriveError {

        str what;
        EVDriveErrorCode_t code;

        VDriveError(std::nullopt_t) {
            code = EVDriveErrorCode_t::STATUS_OKAY;
            what = "";
        }
        VDriveError() {
            code = EVDriveErrorCode_t::STATUS_OKAY;
            what = "";
        }

        VDriveError(str what, EVDriveErrorCode_t code)
            : what(std::move(what)), code(code) {}


    };


    template <class ...Ts>
    class VDrive;

    template <class ...Ts>
    using EntryToString_pfn_t = str(*)(VDrive<Ts...>*, qk::HObject<Ts...>, EntryDisplayFlags);

    template <class ...Ts>
    class VDrive {
        KernelPermissions m_Permissions;

        public:
            // Lifetime
            VDrive(KernelPermissions);
            virtual ~VDrive() = default;

            // Sets the function to display an entity


            // get permissions
            KernelPermissions Permissions() const;

            // CRUD
            virtual std::expected<qk::HObject<Ts...>, VDriveError> create(const KeyValues&) = 0;
            virtual std::expected<KeyValues, VDriveError> read(qk::HObject<Ts...>, const KeyValues& request) const = 0;
            virtual std::expected<void, VDriveError>  update(qk::HObject<Ts...>, const KeyValues& request) = 0;
            virtual std::expected<bool, VDriveError> destroy(qk::HObject<Ts...>, const KeyValues & = {}) = 0;

            // validation
            virtual std::expected<bool, VDriveError> valid(qk::HObject<Ts...>) const = 0;

            // placeholder...consider a bytecode?
            using Query = u32;

            // look up entity by its CLabel
            virtual std::expected<qk::HObject<Ts...>, VDriveError> lookup(str_view label) const = 0;
            virtual std::expected<std::vector<qk::HObject<Ts...>>, VDriveError> find(const Query&) const = 0;

            virtual str to_string(qk::HObject<Ts...> target, EntryDisplayFlags flags) = 0;



            
            virtual void list(qk::HObject<Ts...> target, const std::vector<str>& keys, EntryDisplayFlags flags) const = 0;

    };

}

#include "VDrive.inl"