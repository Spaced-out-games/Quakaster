#pragma once
#include "KeyValues.h"
#include "Kernel.h"
#include "UURH.h"
#include <expected>
#include <optional>

namespace qk {


    /*
    enum class EntryDisplayFlags : std::uint16_t {
        NONE = 0,
        LONG = 1 << 0,   // ls -l: one entry per line with details
        ALL = 1 << 1,   // ls -a: include hidden entries (unlabeled, internal components)
        PATH = 1 << 2,   // print full path (world:/door_a/cdoor.locked), not just the name
        ID = 1 << 3,   // include the qk::UURH (#3:12v0)
        TYPE = 1 << 4,   // append a type marker (dir/, component, field)
        VALUES = 1 << 5,   // show field values inline (cat-style)
        PERMS = 1 << 6,   // show the effective Permissions mask (ls -l style)
        RECURSIVE = 1 << 7,   // descend into contained entries (-r)
        SORTED = 1 << 8,   // stable sort before printing
        RAW = 1 << 9,   // machine-readable: paths only, one per line, no decoration
    };*/

    enum class EVDriveErrorCode_t : u8 {
        STATUS_OKAY = 0
    };




    class VDrive {
        KernelPermissions m_Permissions;
        public:

        std::unordered_map<str, KernelSyscall_pfn_t> m_pSyscalls;

            // Lifetime
            VDrive(KernelPermissions);
            virtual ~VDrive() = default;

            // Registers this vdrive to the kernel
            virtual void on_mount(Kernel&);

            // Unregisters this vdrive from the kernel
            virtual void on_unmount(Kernel&);

            // issue a syscall to this vdrive
            KernelCallResult syscall(Kernel& k, const str& name, const KeyValues& args);


            // get permissions
            KernelPermissions Permissions() const;

    };

}
