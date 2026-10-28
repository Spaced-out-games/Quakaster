namespace qk {

	VDrive::VDrive(KernelPermissions perms)
		: m_Permissions(perms)
	{}

	KernelPermissions VDrive::Permissions() const {
		return m_Permissions;
	}

}