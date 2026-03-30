/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/utility/ServiceManager
/// Purpose: Defines a service registry type, capable of integrating with your own 
/// custom service storage sysetm
/// **************************************************************************************************

#pragma once
#include <unordered_map>
#include <string>
#include "Service.h"
#include "spdlog/spdlog.h"

namespace qk::util {

    struct ServiceEntry;
    using ServiceMap = std::unordered_map<std::string, ServiceEntry>;

    struct ServiceEntry {
        IService* service = nullptr;
        void (*dtor)(void* storage, ServiceMap&, const std::string&) = nullptr; // C-style function pointer
    };

    // Type for ServiceManager bookkeeping

    // ----------------------------------------
    // Concept: runtime service storage
    // ----------------------------------------
    template <typename S>
    concept ServiceStorageConcept = requires(S s, ServiceMap& map, const std::string & name) {
        // emplace<T>(name, map, args...) -> returns IService*
        { s.template emplace<int>(name, map) } -> std::same_as<IService*>;

        // get(name, map) -> returns IService*
        { s.get(name, map) } -> std::same_as<IService*>;

        // erase(name, map) -> void
        { s.erase(name, map) } -> std::same_as<void>;
    };

    // ----------------------------------------
    // ServiceManager wraps any storage
    // ----------------------------------------
    template <ServiceStorageConcept Storage>
    struct BasicServiceManager {
        Storage* storage;
        ServiceMap map;

        template <typename T, typename... Args>
        T* emplace(const std::string& name, Args&&... args) {
            spdlog::info("Added service {} to ServiceManager", name);
            return static_cast<T*>(storage->template emplace<T>(name, map, std::forward<Args>(args)...));
        }

        IService* get(const std::string& name) {
            
            return storage->get(name, map);
        }

        
        void erase(const std::string& name) {
            spdlog::info("Removed service {} from ServiceManager", name);
            storage->erase(name, map);
        }
    };



    


} // namespace qk