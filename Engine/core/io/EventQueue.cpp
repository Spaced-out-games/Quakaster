/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/io/EventQueue.cpp
/// Purpose: Implements and event queue
/// ************************************************************************************************** 

#include "EventQueue.h"


namespace qk::io {
    EventQueue::EventQueue() {
        m_Data.reserve(256);
    }

    void EventQueue::push_back(const Event& e) {
        m_Data.push_back(e);
    }

    void EventQueue::push_back(Event&& e) {
        m_Data.push_back(std::move(e));
    }

    const std::vector<Event>& EventQueue::events() {
        return m_Data;
    }


    void EventQueue::clear() {
        m_Data.clear();
    }

    size_t EventQueue::size() const {
        return m_Data.size();
    }
    bool EventQueue::empty() const {
        return m_Data.empty();
    }


    Event* EventQueue::data() { return m_Data.data(); }
    const Event* EventQueue::data() const { return m_Data.data(); }

    auto EventQueue::begin() { return m_Data.begin(); }
    auto EventQueue::end() { return m_Data.end(); }

}