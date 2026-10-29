#pragma once
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <type_traits>
#include <unordered_map>
#include "spdlog/spdlog.h"
#include <core.h>
#include "Port.h"
namespace qk {



	using SignalID = u64;

	using SignalListener_pfn_t = void(*)(entt::registry& reg, entt::entity src, entt::entity dest, SignalID signal_ID);

	// todo: evolve this type
	//using CSignalListener = 


    struct CSignalListener {
        std::unordered_map<SignalID, SignalListener_pfn_t> m_Map;

        public:
            void addListener(SignalID, SignalListener_pfn_t);
            void removeListener(SignalID);
            bool callListener(entt::registry&, entt::entity, entt::entity, SignalID);


    };



    template <typename T>
    void write_port(entt::registry& reg, entt::entity dest, T&& value)
    {
        static_assert(std::is_trivially_copyable_v<T>);
        if (auto* pPort = reg.try_get<CPort<std::decay_t<T>>>(dest)) {
            pPort->inbox = std::forward<T>(value);
        }
        else {

            #ifdef _DEBUG
                spdlog::warn("Entity {} does not have a CPort<{}>", dest, typeid(T).name());
            #endif

        }
    }


    template <typename ...Ts>
    void send(entt::registry& reg, entt::entity src, entt::entity dest, SignalID signal, Ts... args) {
        
        (write_port(reg, dest, args), ...);

        CSignalListener* pSignalListener = reg.try_get<CSignalListener>(dest);

        // not a hard error but in debug builds treat it as such to catch programmer error
        assert(pSignalListener);
        if (!pSignalListener)
            return;


        // dispatch
        pSignalListener->callListener(reg, src, dest, signal);

    }

}