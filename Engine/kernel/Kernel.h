#pragma once
#include <vector>
#include <memory>

namespace qk {

    template <class ...Ts>
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

    template <class ...ID_Ts>
	struct Kernel {
        using Drive_t = VDrive<ID_Ts...>;
		std::vector<Drive_t*> m_VDrives;


	};

}

