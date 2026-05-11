/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/io/Event.h
/// Purpose: Handles Quakaster event destruction and callback setup functionality.
/// ************************************************************************************************** 
#include "Event.h"

namespace qk::io {

	EventEx_dtor_pfn_t g_EventEx_dtor = nullptr;

	Event::~Event() {

		switch (m_Type)
		{
		default:// core implementations
			break;
		}
		// give control to the extension callback
		if ((uint32_t)m_Type >= CUSTOM_EVENT_BEGIN && g_EventEx_dtor) g_EventEx_dtor(*this);


	}

	void SetEventExDtor_fn(EventEx_dtor_pfn_t dtor) {
		g_EventEx_dtor = dtor;
	}

}