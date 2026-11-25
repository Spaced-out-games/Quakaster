#pragma once
#include <vector>
#include <memory>
#include <KeyValues.h>

namespace qk {
    
    class VDrive;

    enum class KernelPermissions : std::uint8_t {
        NONE = 0,
        ENTRY_CREATE = 0b1, // invoker can create entries
        ENTRY_READ = 0b10, // invoker can read entries
        ENTRY_WRITE = 0b100, // invoker can write / update entries
        ENTRY_DESTROY = 0b1000, // invoker can destroy entries
        PORT_SEND = 0b10000, // invoker can write to ports
        PORT_RECEIVE = 0b100000  // invoker can reaf from ports

    };

    enum EKernelStatus_t: u32 {
        KERNEL_STATUS_SUCCESS = 0,
        KERNEL_STATUS_INVALID_ARGUMENTS,
        KERNEL_STATUS_UNKNOWN_COMMAND,
        KERNEL_STATUS_VDRIVE_ACCESS_DENIED,
        KERNEL_INVALID_RESOURCE,
    };


    struct KernelCallResult {
        EKernelStatus_t status;
        KeyValues result;
    };



    struct Kernel;

    using KernelSyscall_pfn_t = KernelCallResult(*)(Kernel&, VDrive*,const KeyValues&);

    template <auto Fn, class... Ts>
    concept KernelCallable = requires {
        {
            std::invoke(
                Fn,
                std::declval<Kernel&>(),
                std::declval<VDrive*>(),
                std::declval<const Ts&>()...
            )
        } -> std::same_as<KernelCallResult>;
    };

    template <auto Fn, class... Ts, size_t... I>
        requires KernelCallable<Fn, Ts...>
    KernelCallResult kernel_trampoline_impl(
        Kernel& kernel,
        VDrive* pVDrive,
        const KeyValues& args,
        std::index_sequence<I...>
    ) {
        if (!(std::holds_alternative<Ts>(args[I]) && ...))
            return KernelCallResult{ KERNEL_STATUS_INVALID_ARGUMENTS, KeyValues() };

        return std::invoke(Fn, kernel, pVDrive, std::get<Ts>(args[I])...);
    }

    template <auto Fn, class... Ts>
        requires KernelCallable<Fn, Ts...>
    KernelCallResult kernel_trampoline(
        Kernel& kernel,
        VDrive* pVDrive,
        const KeyValues& args
    ) {
        if (args.size() != sizeof...(Ts))
            return KernelCallResult{ KERNEL_STATUS_INVALID_ARGUMENTS, KeyValues() };

        return kernel_trampoline_impl<Fn, Ts...>(
            kernel,
            pVDrive,
            args,
            std::index_sequence_for<Ts...>{}
        );
    }


	struct Kernel {
		std::vector<VDrive*> m_VDrives;



        std::unordered_map<str, KernelSyscall_pfn_t> m_SyscallLookup;

	};

}

