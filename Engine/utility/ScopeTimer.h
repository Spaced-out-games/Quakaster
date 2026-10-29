/// **************************************** QUAKASTER ENGINE **************************************** 
/// utility/ScopeTimer.h
/// Purpose: Defines a type that records how much time passes between construction and destruction,
/// outputting to some time aggregator
/// **************************************************************************************************
#pragma once
#include <stdint.h>
#include "Profiler.h"
#include "core.h"

namespace qk::utility {
    // Times the duration of a scope
    struct QK_API ScopeTimer {
        const char* m_pMsg = nullptr;
        uint64_t m_StartTime = 0;
        Profiler* m_pProfiler = nullptr;

        explicit ScopeTimer(const char* pMsg, Profiler* pProfiler = nullptr);

        ~ScopeTimer();

        ScopeTimer(const ScopeTimer&) = delete;
        ScopeTimer& operator=(const ScopeTimer&) = delete;

        ScopeTimer(ScopeTimer&& other) noexcept;
    };
}