#pragma once

namespace qk
{

    template<typename Resource_t>
    Resource_t* ResourceRegistry<Resource_t>::insert(
        str_view label,
        Resource_t&& resource)
    {
        const u64 id =
            qk::hash_fnv1a(label);

        resource.id = id;
        resource.refs = 0;
        resource.label = label;

        auto [it, inserted] =
            m_Resources.try_emplace(
                id,
                std::move(resource));

        if (!inserted)
        {
            QK_ASSERT(
                it->second.label == label &&
                "Hash collision detected.");

            return nullptr;
        }

        return &it->second;
    }

    template<typename Resource_t>
    bool ResourceRegistry<Resource_t>::destroy(
        Resource_t* resource)
    {
        if (!resource)
            return false;

        auto it =
            m_Resources.find(resource->id);

        if (it == m_Resources.end())
            return false;

        if (it->second.refs != 0)
            return false;

        m_Resources.erase(it);

        return true;
    }

    template<typename Resource_t>
    bool ResourceRegistry<Resource_t>::valid(
        const Resource_t* resource) const
    {
        if (!resource)
            return false;

        auto it =
            m_Resources.find(resource->id);

        return it != m_Resources.end();
    }

    template<typename Resource_t>
    Resource_t* ResourceRegistry<Resource_t>::find(
        str_view label)
    {
        const u64 id =
            qk::hash_fnv1a(label);

        auto it =
            m_Resources.find(id);

        if (it == m_Resources.end())
            return nullptr;

        QK_ASSERT(
            it->second.label == label);

        return &it->second;
    }

    template<typename Resource_t>
    const Resource_t* ResourceRegistry<Resource_t>::find(
        str_view label) const
    {
        const u64 id =
            qk::hash_fnv1a(label);

        auto it =
            m_Resources.find(id);

        if (it == m_Resources.end())
            return nullptr;

        QK_ASSERT(
            it->second.label == label);

        return &it->second;
    }

    template<typename Resource_t>
    Resource_t* ResourceRegistry<Resource_t>::find(
        u64 id)
    {
        auto it =
            m_Resources.find(id);

        if (it == m_Resources.end())
            return nullptr;

        return &it->second;
    }

    template<typename Resource_t>
    const Resource_t* ResourceRegistry<Resource_t>::find(
        u64 id) const
    {
        auto it =
            m_Resources.find(id);

        if (it == m_Resources.end())
            return nullptr;

        return &it->second;
    }

    template<typename Resource_t>
    bool ResourceRegistry<Resource_t>::acquire(
        const Resource_t* resource)
    {
        if (!resource)
            return false;

        auto it =
            m_Resources.find(resource->id);

        if (it == m_Resources.end())
            return false;

        ++it->second.refs;

        return true;
    }

    template<typename Resource_t>
    bool ResourceRegistry<Resource_t>::release(
        const Resource_t* resource)
    {
        if (!resource)
            return false;

        auto it =
            m_Resources.find(resource->id);

        if (it == m_Resources.end())
            return false;

        if (it->second.refs == 0)
        {
            QK_ASSERT(false);
            return false;
        }

        --it->second.refs;

        return true;
    }

    template<typename Resource_t>
    int ResourceRegistry<Resource_t>::refcount(
        const Resource_t* resource) const
    {
        if (!resource)
            return -1;

        auto it =
            m_Resources.find(resource->id);

        if (it == m_Resources.end())
            return -1;

        return static_cast<int>(
            it->second.refs);
    }

    template<typename Resource_t>
    str_view ResourceRegistry<Resource_t>::label(
        const Resource_t* resource) const
    {
        if (!resource)
            return {};

        auto it =
            m_Resources.find(resource->id);

        if (it == m_Resources.end())
            return {};

        return it->second.label;
    }

    template<typename Resource_t>
    u64 ResourceRegistry<Resource_t>::id(
        const Resource_t* resource) const
    {
        if (!resource)
            return UINT64_MAX;

        return resource->id;
    }

    template<typename Resource_t>
    void ResourceRegistry<Resource_t>::clear()
    {
        m_Resources.clear();
    }

    template<typename Resource_t>
    size_t ResourceRegistry<Resource_t>::size() const
    {
        return m_Resources.size();
    }

}