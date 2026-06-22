/// **************************************** QUAKASTER ENGINE **************************************** 
/// utility/Stopwatch.h
/// Purpose: Times things
/// **************************************************************************************************
#pragma once
#include "core.h"
#include <stdint.h>
#include "../utility/time.h"

namespace qk::utility {

	// Times a process
	struct QK_API Stopwatch {
		uint64_t m_StartTime = 0;
		uint64_t m_TotalTime = 0;

		// Start the timer
		void start();

		// Stop the timer
		void stop();

		// Reset the timer
		void reset();

		// Get the total time elapsed between each start-stop cycle in nanoseconds
		[[nodiscard]] uint64_t totalTime() const;

		// Get the total time elapsed between each start-stop cycle in milliseconds
		[[nodiscard]] double totalTime_ms() const;

		// Get the total time elapsed between each start-stop cycle in seconds
		[[nodiscard]] double totalTime_s() const;
	};

}