/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/io/EventQueue.h
/// Purpose: Defines a queue for storing Events
/// ************************************************************************************************** 


#pragma once
#include "Event.h"
#include <vector>
#include "../../core.h"


namespace qk::io {
    struct QK_API EventQueue {
    private:
        std::vector<Event> m_Data;

    public:
        EventQueue();

        // --- vector-like API ---

        void push_back(const Event& e);
        void push_back(Event&& e);

        template<typename... Args>
        Event& emplace_back(Args&&... args) {
            return m_Data.emplace_back(std::forward<Args>(args)...);
        }

        const std::vector<Event>& events();

        void clear();

        size_t size() const;
        bool empty() const;

        Event& operator[](size_t i) { return m_Data[i]; }
        const Event& operator[](size_t i) const { return m_Data[i]; }

        Event* data();
        const Event* data() const;

        // Iteration
        auto begin();
        auto end();
    };
}