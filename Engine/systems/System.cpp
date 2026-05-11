#include "System.h"
#include "../utility/time.h"

using qk::utility::now;

namespace qk {
	void System::set_tick_interval_ns(uint64_t interval_ns) {
		m_TickInterval_ns = interval_ns;
	}

	uint64_t System::get_tick_interval_ns() {
		return m_TickInterval_ns;
	}

	bool System::valid() {
		return m_Implementation != nullptr;
	}

	bool System::ready() {
		return m_Implementation && (now() >= m_NextTick_ns);
	}



	void System::operator()() {

		if (!m_Implementation || !ready()) return;
		m_NextTick_ns += m_TickInterval_ns;
		m_Implementation();

	}
}