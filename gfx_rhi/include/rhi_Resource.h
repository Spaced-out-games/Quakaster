#pragma once
#include <unordered_map>
#include <type_traits>
#include <fnv1a.h>
#include <core.h>
#define RHI_USE_VALIDATION_LAYERS 1


namespace qk::gfx::rhi {

	constexpr u64 NULL_RESOURCE = UINT64_MAX;


	class Resource {
	public:
		operator u64() const;

        Resource();
		Resource(u64 id);
		Resource(qk::string_hash label);

		Resource& operator=(u64) = delete;
		Resource& operator=(const qk::string_hash&) = delete;

		bool operator==(const Resource&) const = default;

		explicit operator bool() const;

	private:
		u64 hResource = NULL_RESOURCE;
	};

	template <class T, class Resource_t>
	class ResourceCache {
		static_assert(std::is_base_of_v<Resource, Resource_t>);

		struct Node {
			T value{};
			str label;
			u32 refcount = 0;
		};

	public:
		Resource_t insert(str_view label, const T& value);

		bool valid(const Resource_t&) const;

		const Node* try_get_node(const Resource_t&) const;
		Node* try_get_node(const Resource_t&);

		const T* try_get(const Resource_t&) const;
		T* try_get(const Resource_t&);

		str_view label(const Resource_t&) const;

		int refcount(const Resource_t&) const;

		bool acquire(const Resource_t&);
		bool release(const Resource_t&);
		bool destroy(const Resource_t&);

	private:
		std::unordered_map<u64, Node> m_Resources;
	};

}



// Implementations




namespace qk::gfx::rhi {

    template <class T, class Resource_t>
    Resource_t ResourceCache<T, Resource_t>::insert(
        str_view label,
        const T& value)
    {
        u64 id = (u64)qk::hash_fnv1a(label);

        auto [it, inserted] =
            m_Resources.try_emplace(
                id,
                Node{ value, str(label), 1 });

        if (!inserted)
        {
            // hash collision detection
            if (it->second.label != label)
            {
                QK_ASSERT(false);
            }

            return Resource_t{ NULL_RESOURCE };
        }

        return Resource_t{ id };
    }

    template <class T, class Resource_t>
    bool ResourceCache<T, Resource_t>::valid(
        const Resource_t& hResource) const
    {
        if (!hResource)
            return false;

        return m_Resources.contains(hResource);
    }

    template <class T, class Resource_t>
    const typename ResourceCache<T, Resource_t>::Node*
        ResourceCache<T, Resource_t>::try_get_node(
            const Resource_t& hResource) const
    {
        auto it = m_Resources.find(hResource);

        if (it == m_Resources.end())
            return nullptr;

        return &it->second;
    }

    template <class T, class Resource_t>
    typename ResourceCache<T, Resource_t>::Node*
        ResourceCache<T, Resource_t>::try_get_node(
            const Resource_t& hResource)
    {
        auto it = m_Resources.find(hResource);

        if (it == m_Resources.end())
            return nullptr;

        return &it->second;
    }

    template <class T, class Resource_t>
    const T* ResourceCache<T, Resource_t>::try_get(
        const Resource_t& hResource) const
    {
        auto it = m_Resources.find(hResource);

        if (it == m_Resources.end())
            return nullptr;

        return &it->second.value;
    }

    template <class T, class Resource_t>
    T* ResourceCache<T, Resource_t>::try_get(
        const Resource_t& hResource)
    {
        auto it = m_Resources.find(hResource);

        if (it == m_Resources.end())
            return nullptr;

        QK_ASSERT(valid(hResource));

        return &it->second.value;
    }

    template <class T, class Resource_t>
    str_view ResourceCache<T, Resource_t>::label(
        const Resource_t& hResource) const
    {
        const Node* node = try_get_node(hResource);

        if (!node)
            return {};

        return node->label;
    }

    template <class T, class Resource_t>
    int ResourceCache<T, Resource_t>::refcount(
        const Resource_t& hResource) const
    {
        const Node* node = try_get_node(hResource);

        if (!node)
            return -1;

        return static_cast<int>(node->refcount);
    }

    template <class T, class Resource_t>
    bool ResourceCache<T, Resource_t>::acquire(
        const Resource_t& hResource)
    {
        Node* node = try_get_node(hResource);

        if (!node)
            return false;

        ++node->refcount;

        return true;
    }

    template <class T, class Resource_t>
    bool ResourceCache<T, Resource_t>::release(
        const Resource_t& hResource)
    {
        Node* node = try_get_node(hResource);

        if (!node)
            return false;

        if (node->refcount == 0)
        {
            QK_ASSERT(false);
            return false;
        }

        --node->refcount;

        return true;
    }

    template <class T, class Resource_t>
    bool ResourceCache<T, Resource_t>::destroy(
        const Resource_t& hResource)
    {
        Node* node = try_get_node(hResource);

        if (!node)
            return false;

        if (node->refcount != 0)
            return false;

        m_Resources.erase(hResource);

        return true;
    }

}