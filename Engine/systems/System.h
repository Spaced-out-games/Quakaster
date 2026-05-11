/// **************************************** QUAKASTER ENGINE **************************************** 
/// ent/System.h
/// Purpose: Defines an interface fpr timed entt interactions.
/// **************************************************************************************************
#pragma once
#include <functional>
#include <vector>
#include "core.h"

namespace qk {

	using System_impl_t = std::function<void(void)>;

	class QK_API System {
		System_impl_t	m_Implementation = nullptr;
		uint64_t		m_TickInterval_ns = 0;
		uint64_t		m_NextTick_ns = 0;
	public:
		void set_tick_interval_ns(uint64_t interval_ns);
		uint64_t get_tick_interval_ns();
		bool valid();
		bool ready();

		void operator()();
	};




}