/// **************************************** QUAKASTER ENGINE **************************************** 
/// gfx/CMeshInstance.h
/// Purpose: 
/// **************************************************************************************************
#pragma once

namespace qk::gfx {


	struct CMeshInstance
	{
		unsigned int m_MeshType = 0;
		unsigned int m_MeshID = 0;
		unsigned int m_InstanceIndex = 0;
	};

}