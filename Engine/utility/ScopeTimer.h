/// **************************************** QUAKASTER ENGINE **************************************** 
/// utility/ScopeTimer.h
/// Purpose: Defines a type that records how much time passes between construction and destruction,
/// outputting to some time aggregator
/// **************************************************************************************************
#pragma once
#include <stdint.h>
#include "core.h"

namespace qk::utility {
    // Times the duration of a scope
    struct QK_API ScopeTimer {
        uint64_t* m_pTarget = nullptr;
        uint64_t m_StartTime = 0;

        explicit ScopeTimer(uint64_t* pTarget);

        ~ScopeTimer();

        ScopeTimer(const ScopeTimer&) = delete;
        ScopeTimer& operator=(const ScopeTimer&) = delete;

        ScopeTimer(ScopeTimer&& other) noexcept;
        ScopeTimer& operator=(ScopeTimer&& other) noexcept;
    };
}