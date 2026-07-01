#include "CSignalListener.h"

namespace qk {
	void signal(entt::registry& reg, entt::entity src, entt::entity dest, string_hash sig) {
		if (!reg.all_of<CSignalListener>(dest)) return;

		reg.get<CSignalListener>(dest)(reg, src, dest, sig);

	}

	void CSignalListener::operator()(entt::registry& reg, entt::entity src, entt::entity dest, string_hash sig) {
		if (!SignalListener_pfn) return;
		SignalListener_pfn(reg, src, dest, sig);
	}

	CSignalListener::CSignalListener(SignalListener_pfn_t listener) {
		SignalListener_pfn = listener;
	}



}