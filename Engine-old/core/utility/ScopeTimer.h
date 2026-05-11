/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/utility/ScopeTimer.h
/// Purpose: Defines a type that records how much time passes between construction and destruction,
/// outputting to some time aggregator
/// **************************************************************************************************
#pragma once
#include <stdint.h>
#include "../../core.h"
namespace qk::util {
    struct QK_API ScopeTimer {
        uint64_t* m_Target = nullptr;
        uint64_t m_StartTime = 0;

        explicit ScopeTimer(uint64_t* target_out);

        ~ScopeTimer();

        // non-copyable
        ScopeTimer(const ScopeTimer&) = delete;
        ScopeTimer& operator=(const ScopeTimer&) = delete;

        ScopeTimer(ScopeTimer&& other) noexcept;
        ScopeTimer& operator=(ScopeTimer&& other) noexcept;
    };
}