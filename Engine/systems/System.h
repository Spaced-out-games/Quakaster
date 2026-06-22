/// **************************************** QUAKASTER ENGINE **************************************** 
/// ent/System.h
/// Purpose: Defines an interface for timed entt interactions.
/// **************************************************************************************************
#pragma once
#include <functional>
#include <vector>
#include "core.h"

namespace qk {

	using System_impl_t = std::function<void(void)>;

	// Timed function pointer, basically.
	class QK_API System {
		System_impl_t	m_Implementation = nullptr;
		uint64_t		m_TickInterval_ns = 0;
		uint64_t		m_NextTick_ns = 0;
	public:
		// 
		void setTickInterval_ns(uint64_t interval_ns);
		uint64_t getTickInterval_ns();
		// Whether or not this system is executable
		bool valid();

		//Whether or not this system is executable and ready for a tick
		bool ready();

		// Calls this system
		void operator()();
	};




}