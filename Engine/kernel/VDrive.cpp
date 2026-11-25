#include "VDrive.h"


namespace qk {

	void VDrive::on_mount(Kernel&) {}
	void VDrive::on_unmount(Kernel&) {}


	KernelCallResult VDrive::syscall(Kernel& k, const str& name, const KeyValues& args) {

		auto it = m_pSyscalls.find(name);
		if (it != m_pSyscalls.end()) {
			return it->second(k, this, args);
		}

		return KernelCallResult{
				KERNEL_STATUS_UNKNOWN_COMMAND,
				KeyValues()
		};

	}

	VDrive::VDrive(KernelPermissions permissions) {
		m_Permissions = permissions;
	}

	KernelPermissions VDrive::Permissions() const {
		return m_Permissions;
	}

}