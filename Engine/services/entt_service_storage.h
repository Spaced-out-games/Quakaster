/// **************************************** QUAKASTER ENGINE **************************************** 
/// services/entt_service_storage.h
/// Purpose: entt implementation of ServiceManager storage.
/// **************************************************************************************************

#pragma once
#include "core.h"
#include <string>
#include <unordered_map>
#include "../services/IService.h"
#include "../utility/ServiceManager.h"
#include <entt/entity/registry.hpp>


namespace qk::service {
    struct entt_service_storage {
        entt::registry* reg;

        template <typename T, typename... Args>
        IService* emplace(const std::string& name, ServiceMap& map, Args&&... args) {
            T& svc = reg->ctx().emplace<T>(std::forward<Args>(args)...);
            map[name].service = &svc;
            map[name].dtor = [](void* storage, ServiceMap&, const std::string&) {
                entt::registry* registry = (entt::registry*)storage;
                registry->ctx().erase<T>();
            };

            return &svc;
        }

        IService* get(const std::string& name, ServiceMap& map) {
            auto it = map.find(name);
            return it != map.end() ? it->second.service : nullptr;
        }


        void erase(const std::string& name, ServiceMap& map) {
            auto it = map.find(name);
            if (it != map.end()) {

                auto dtor = it->second.dtor;

                if (dtor) dtor(reg, map, name);
                map.erase(it);
            }
        }
    };
}