/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/utility/Stopwatch.cpp
/// Purpose: Stopwatch implementation.
/// **************************************************************************************************
#include "Stopwatch.h"


namespace qk::util {



	void Stopwatch::start() {
		m_StartTime = now();
	}
	void Stopwatch::stop() {
		if (m_StartTime == 0) return;
		m_TotalTime += now() - m_StartTime;
		m_StartTime = 0;
	}
	void Stopwatch::reset() {
		m_TotalTime = 0;
		m_StartTime = 0;
	}

	uint64_t Stopwatch::total_time() const { return m_TotalTime; }
	double Stopwatch::total_time_ms() const { return m_TotalTime / 1e6; }
	double Stopwatch::total_time_s() const { return m_TotalTime / 1e9; }



}