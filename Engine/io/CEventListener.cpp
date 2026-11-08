#include "CEventListener.h"

namespace qk {

	CEventListener::CEventListener(OnEventListener_pfn_t impl) : m_Impl(impl) {}

	CEventListener& CEventListener::operator=(OnEventListener_pfn_t impl) {
		m_Impl = impl;
		return *this;
	}


	void CEventListener::operator()(entt::entity target, entt::registry& registry, const Event& evt) {
		QK_ASSERT(m_Impl != nullptr, "m_Impl was nullptr");
		// recoverable if no impl
		if (target == entt::null || m_Impl == nullptr)
			return;

		m_Impl(target, registry, evt);

	}


}