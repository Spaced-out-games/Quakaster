#include "Signal.h"


namespace qk {


    void CSignalListener::addListener(SignalID signal, SignalListener_pfn_t listener) {
        // no double-inserts
        assert(m_Map.find(signal) == m_Map.end());

        m_Map.at(signal) = listener;

    }
    void CSignalListener::removeListener(SignalID signal) {
        assert(m_Map.find(signal) != m_Map.end());
        m_Map.erase(signal);
    }
    bool CSignalListener::callListener(entt::registry& reg, entt::entity src, entt::entity dest, SignalID signal) {

        auto it = m_Map.find(signal);

        if (it == m_Map.end())
            return false;
        
        it->second(reg, src, dest, signal);

        return true;
        
    }
}