/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/utility/Service.cpp
/// Purpose: Adds logging and trivial operations for the Service interface
/// **************************************************************************************************

#include "IService.h"
#include "spdlog/spdlog.h"

namespace qk::service {

	size_t IService::uptime_ns() { return m_Uptime_ns; }

	size_t* IService::uptime_ptr() { return &m_Uptime_ns; }


	void IService::init() {
		spdlog::info("Service initialized at {:#x}", (uintptr_t)this);

	}
	void IService::shutdown() {
		spdlog::info("Service shutdown at {:#x}", (uintptr_t)this);

	}

	void IService::clearTimer() {
		m_Uptime_ns = 0;
	}


}