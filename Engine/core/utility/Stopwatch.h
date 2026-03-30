/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/utility/Stopwatch.h
/// Purpose: Times things
/// **************************************************************************************************
#pragma once
#include "../../core.h"
#include <stdint.h>
#include "spdlog/spdlog.h"
#include "time.h"

namespace qk::util {


	struct QK_API Stopwatch {
		uint64_t m_StartTime = 0;
		uint64_t m_TotalTime = 0;

		void start();

		void stop();

		void reset();

		[[nodiscard]] uint64_t total_time() const;

		[[nodiscard]] double total_time_ms() const;
		[[nodiscard]] double total_time_s() const;
	};


}