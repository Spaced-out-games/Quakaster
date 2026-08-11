#pragma once
#include "EntityLoader.h"
#include <unordered_set>
#include <spdlog/spdlog.h>

namespace qk {
    void EntityLoader::push_loader(const ComponentLoader& loader) {
        // don't insert bad loaders
        if (loader.loader == nullptr) {
            //spdlog::warn("EntityLoader@{}: Attempted to insert component loader `{}` with no implementation",
            //    (const void*)this,
            //    loader.name
            //);
            return;
        }

        if (loader.dependencies.size() == 0) {
            //spdlog::warn("EntityLoader@{}: Attempted to insert component loader `{}` with no dependency list",
            //    (const void*)this,
            //    loader.name
            //);
            return;
        }

        // check to make sure none of the dependencies exist

        for (const auto& dependency : loader.dependencies) {
            if (m_Loaders.contains(dependency)) {
                //spdlog::warn(
                //    "EntityLoader@{}: Component loader `{}` conflicts on an existing dependency",
                //    (const void*)this,
                //    loader.name
                //);

                return;
            }
        }

        // then push it
        for (const auto& dependency : loader.dependencies) {
            EntityLoaderEntry entry = {
                .name = loader.name,
                .impl = loader.loader
            };

            m_Loaders[dependency] = entry;

        }



    }

    void EntityLoader::push_loaders(std::span<const ComponentLoader> loaders) {
        for (const auto& loader : loaders) {
            push_loader(loader);
        }
    }

    void EntityLoader::execute(
        entt::registry& registry,
        entt::entity entity,
        const EntityArgumentMap& args) const
    {
        std::unordered_set<EntityComponentLoader_pfn_t> used;

        for (auto it = args.begin(); it != args.end(); ++it)
        {
            const auto id = it->first;
            const auto& arg = it->second;

            auto loader = m_Loaders.find(id);

            if (loader == m_Loaders.end())
            {
               spdlog::warn(
                    "EntityLoader@{}: Unknown entity argument `{}`",
                    (const void*)this,
                    id
                );
                continue;
            }

            const auto& entry = loader->second;

            if (!used.insert(entry.impl).second)
                continue;

            entry.impl(registry, entity, args);
        }
    }
}