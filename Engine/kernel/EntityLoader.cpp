#pragma once

#include "EntityLoader.h"

#include <unordered_set>
#include <spdlog/spdlog.h>

namespace qk {

    void EntityLoader::push_loader(const ComponentLoader& loader) {
        // Don't insert invalid loaders.
        if (loader)
            return;

        if (loader.num_dependencies() == 0)
            return;

        // Check all dependencies before inserting anything.
        for (const auto& dependency : loader.dependencies()) {
            if (m_Loaders.contains(dependency))
                return;
        }

        // Insert the loader for each dependency.
        for (const auto& dependency : loader.dependencies()) {


            m_Loaders[dependency] = EntityLoaderEntry{ loader.name(), loader.impl() };
        }
    }


    void EntityLoader::push_loaders(std::span<const ComponentLoader> loaders) {
        for (const auto& loader : loaders)
            push_loader(loader);
    }


    void EntityLoader::execute(
        entt::registry& registry,
        entt::entity entity,
        const KeyValues& args
    ) const {
        std::unordered_set<EntityComponentLoader_pfn_t> used;

        // Iterate over named arguments.
        for (auto it = args.m_ValueLookup.cbegin();
            it != args.m_ValueLookup.cend();
            ++it)
        {
            const auto& id = it->first;

            auto loader = m_Loaders.find(id);

            if (loader == m_Loaders.end()) {
                spdlog::warn(
                    "EntityLoader@{}: Unknown entity argument `{}`",
                    static_cast<const void*>(this),
                    id
                );

                continue;
            }

            const auto& entry = loader->second;

            // Don't execute the same implementation more than once.
            if (!used.insert(entry.impl).second)
                continue;

            entry.impl(registry, entity, args);
        }
    }

}