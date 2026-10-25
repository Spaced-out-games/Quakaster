namespace qk {

	template <class ...Ts>
	VDrive<Ts...>::VDrive(KernelPermissions perms)
		: m_Permissions(perms)
	{}

	template <class ...Ts>
	KernelPermissions VDrive<Ts...>::Permissions() const {
		return m_Permissions;
	}

}