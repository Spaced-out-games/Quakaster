/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/utility/ScopeTimer.cpp
/// Purpose: Implements ScopeTimer.h
/// **************************************************************************************************

#include "ScopeTimer.h"
#include "spdlog/spdlog.h"
#include "time.h"

namespace qk::utility {
	ScopeTimer::~ScopeTimer() {
		
		#ifdef _DEBUG
		//spdlog::info("{} took {}ns.", m_pMsg, (now() - m_StartTime));
		#endif
		if(m_pProfiler)
			m_pProfiler->record(m_pMsg, qk::utility::now() - m_StartTime);
		//if (m_pTarget) (*m_pTarget += now() - m_StartTime);
	}

	ScopeTimer::ScopeTimer(const char* pMsg, Profiler* pProfiler) {
		assert(pMsg);
		m_pProfiler = pProfiler;
		m_pMsg = pMsg;
		m_StartTime = now();
	}

}