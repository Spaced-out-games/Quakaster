/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/utility/ScopeTimer.cpp
/// Purpose: Implements ScopeTimer.h
/// **************************************************************************************************

#include "ScopeTimer.h"
#include "spdlog/spdlog.h"
#include "time.h"

namespace qk::utility {
	ScopeTimer::~ScopeTimer() {
		if (m_pTarget) (*m_pTarget += now() - m_StartTime);
	}

	ScopeTimer& ScopeTimer::operator=(ScopeTimer&& other) noexcept {
		if (this != &other) {
			// finalize current timer before overwriting
			if (m_pTarget) {
				*m_pTarget += (now() - m_StartTime);
			}

			m_pTarget = other.m_pTarget;
			m_StartTime = other.m_StartTime;

			other.m_pTarget = nullptr;
		}
		return *this;
	}

	ScopeTimer::ScopeTimer(uint64_t* pTarget) {

		if (!pTarget) spdlog::error("Passed `nullptr` into ScopeTimer at {:#x}", (uintptr_t)this);

		m_pTarget = pTarget;
		m_StartTime = now();
	}

	ScopeTimer::ScopeTimer(ScopeTimer&& other) noexcept : m_pTarget(other.m_pTarget), m_StartTime(other.m_StartTime) { other.m_pTarget = nullptr; }
}