#pragma once
#include "ComponentLoader.h"
#include <span>

namespace qk {

    struct EntityLoaderEntry {
        str_view name = "<anonymous loader>";
        EntityComponentLoader_pfn_t impl = nullptr;
    };

    class EntityLoader {
        emhash5::HashMap<
            EntityComponentArgumentID,
            EntityLoaderEntry
        > m_Loaders;

    public:
        void push_loader(const ComponentLoader& loader);

        void push_loaders(std::span<const ComponentLoader> loaders);

        void execute(entt::registry& registry, entt::entity entity, const EntityArgumentMap& args) const;


    };

}