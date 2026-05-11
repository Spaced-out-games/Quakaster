/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/utility/ScopeTimer.cpp
/// Purpose: Implements ScopeTimer.h
/// **************************************************************************************************

#include "ScopeTimer.h"
#include "spdlog/spdlog.h"
#include "time.h"

namespace qk::util {
	ScopeTimer::~ScopeTimer() {
		if (m_Target) (*m_Target += now() - m_StartTime);
	}

	ScopeTimer& ScopeTimer::operator=(ScopeTimer&& other) noexcept {
		if (this != &other) {
			// finalize current timer before overwriting
			if (m_Target) {
				*m_Target += (now() - m_StartTime);
			}

			m_Target = other.m_Target;
			m_StartTime = other.m_StartTime;

			other.m_Target = nullptr;
		}
		return *this;
	}

	ScopeTimer::ScopeTimer(uint64_t* target_out) {

		if (!target_out) spdlog::error("Passed `nullptr` into ScopeTimer at {:#x}", (uintptr_t)this);

		m_Target = target_out;
		m_StartTime = now();
	}

	ScopeTimer::ScopeTimer(ScopeTimer&& other) noexcept : m_Target(other.m_Target), m_StartTime(other.m_StartTime) { other.m_Target = nullptr; }
}