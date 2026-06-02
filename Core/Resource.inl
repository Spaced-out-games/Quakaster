#pragma once

namespace qk
{

    template<typename Resource_t>
    Resource_t* ResourceRegistry<Resource_t>::insert(
        str_view label,
        Resource_t* resource)
    {
        if (!resource)
            return nullptr;

        const u64 label_hash =
            qk::hash_fnv1a(label);

        resource->hash_code = label_hash;
        resource->references = 0;
        resource->label = label;
        resource->state = Active;

        auto [it, inserted] =
            m_Resources.try_emplace(
                label_hash,
                resource);

        if (!inserted)
        {
            QK_ASSERT(
                it->second->label == label &&
                "Hash collision detected.");

            return nullptr;
        }

        return resource;
    }

    template<typename Resource_t>
    bool ResourceRegistry<Resource_t>::destroy(
        Resource_t*& resource)
    {
        if (!resource)
            return false;

        auto it =
            m_Resources.find(
                resource->hash_code);

        if (it == m_Resources.end())
            return false;

        if (!resource->ready_for_deletion())
            return false;

        resource->state = Destroyed;

        m_Resources.erase(it);

        resource = nullptr;

        return true;
    }

    template<typename Resource_t>
    bool ResourceRegistry<Resource_t>::valid(
        const Resource_t* resource) const
    {
        if (!resource)
            return false;

        auto it =
            m_Resources.find(
                resource->hash_code);

        if (it == m_Resources.end())
            return false;

        return it->second == resource;
    }

    template<typename Resource_t>
    Resource_t* ResourceRegistry<Resource_t>::find(
        str_view label)
    {
        const u64 label_hash =
            qk::hash_fnv1a(label);

        auto it =
            m_Resources.find(label_hash);

        if (it == m_Resources.end())
            return nullptr;

        QK_ASSERT(
            it->second->label == label);

        return it->second;
    }

    template<typename Resource_t>
    const Resource_t* ResourceRegistry<Resource_t>::find(
        str_view label) const
    {
        const u64 label_hash =
            qk::hash_fnv1a(label);

        auto it =
            m_Resources.find(label_hash);

        if (it == m_Resources.end())
            return nullptr;

        QK_ASSERT(
            it->second->label == label);

        return it->second;
    }

    template<typename Resource_t>
    Resource_t* ResourceRegistry<Resource_t>::find(
        u64 id)
    {
        auto it =
            m_Resources.find(id);

        if (it == m_Resources.end())
            return nullptr;

        return it->second;
    }

    template<typename Resource_t>
    const Resource_t* ResourceRegistry<Resource_t>::find(
        u64 id) const
    {
        auto it =
            m_Resources.find(id);

        if (it == m_Resources.end())
            return nullptr;

        return it->second;
    }

    template<typename Resource_t>
    bool ResourceRegistry<Resource_t>::acquire(
        const Resource_t* resource)
    {
        if (!resource)
            return false;

        auto it =
            m_Resources.find(
                resource->hash_code);

        if (it == m_Resources.end())
            return false;

        ++it->second->references;

        return true;
    }

    template<typename Resource_t>
    bool ResourceRegistry<Resource_t>::release(
        const Resource_t* resource)
    {
        if (!resource)
            return false;

        auto it =
            m_Resources.find(
                resource->hash_code);

        if (it == m_Resources.end())
            return false;

        if (it->second->references == 0)
        {
            QK_ASSERT(false);
            return false;
        }

        --it->second->references;

        return true;
    }

    template<typename Resource_t>
    int ResourceRegistry<Resource_t>::refcount(
        const Resource_t* resource) const
    {
        if (!resource)
            return -1;

        auto it =
            m_Resources.find(
                resource->hash_code);

        if (it == m_Resources.end())
            return -1;

        return static_cast<int>(
            it->second->references);
    }

    template<typename Resource_t>
    str_view ResourceRegistry<Resource_t>::label(
        const Resource_t* resource) const
    {
        if (!resource)
            return {};

        auto it =
            m_Resources.find(
                resource->hash_code);

        if (it == m_Resources.end())
            return {};

        return it->second->label;
    }

    template<typename Resource_t>
    u64 ResourceRegistry<Resource_t>::id(
        const Resource_t* resource) const
    {
        if (!resource)
            return UINT64_MAX;

        return resource->hash_code;
    }

    template<typename Resource_t>
    void ResourceRegistry<Resource_t>::clear()
    {
        for (auto& [id, resource] : m_Resources)
        {
            if (resource)
                resource->state = Destroyed;
        }

        m_Resources.clear();
    }

    template<typename Resource_t>
    size_t ResourceRegistry<Resource_t>::size() const
    {
        return m_Resources.size();
    }

}