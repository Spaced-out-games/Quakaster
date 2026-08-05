#include "CEventListener.h"

namespace qk {

	CEventListener::CEventListener(OnEventListener_pfn_t impl) : m_Impl(impl) {}

	CEventListener& CEventListener::operator=(OnEventListener_pfn_t impl) {
		m_Impl = impl;
		return *this;
	}


	void CEventListener::operator()(entt::entity target, entt::registry& registry, const Event& evt) {
		#ifdef _DEBUG
		assert(m_Impl != nullptr);
		#endif
		// recoverable if no impl
		if (target == entt::null || m_Impl == nullptr)
			return;

		m_Impl(target, registry, evt);

	}


}