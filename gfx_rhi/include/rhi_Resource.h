#pragma once
#include <unordered_map>
#include <type_traits>
#include <fnv1a.h>
#include <core.h>
#define RHI_USE_VALIDATION_LAYERS 1

namespace qk::gfx::rhi {

	constexpr u64 NULL_RESOURCE = UINT64_MAX;

	class Resource {
		
		u64 hResource = NULL_RESOURCE;
		public:
			operator u64() const { return hResource; }

			Resource(u64 id): hResource(id) {}
			Resource() = default;

			Resource(qk::string_hash label): hResource(label.hash) {}

			Resource& operator=(u64) = delete;
			Resource& operator=(const qk::string_hash&) = delete;

			bool operator==(const Resource&) const = default;

			explicit operator bool() const {
				return hResource != NULL_RESOURCE;
			}

	};
	/*
	template <class To_t, class From_t>
	To_t Resource_cast(const From_t& from) {

		static_assert(std::is_base_of_v<Resource, To_t>,
			"To must derive from Resource");

		static_assert(std::is_base_of_v<Resource, From_t>,
			"From must derive from Resource");

		return To{(u64)from};
	}
	*/
	template <class T, class Resource_t>
	class ResourceCache {
		static_assert(std::is_base_of_v<Resource, Resource_t>, "Resource_t must derive from Resource");

		struct Node {
			T			value{};
			str label;
			u32 refcount = 0;
		};


		std::unordered_map<u64, Node> m_Resources;

		public:

			Resource_t insert(str_view label, const T& value) {
				u64 id = (u64)qk::hash_fnv1a(label);
				auto [it, inserted] = m_Resources.try_emplace(id, Node{ value,str(label) ,1 });
				///*
				if (!inserted)
				{
					// hash collision detection
					if (it->second.label != label) {
						QK_ASSERT(false);
					}
					return Resource_t{ NULL_RESOURCE };
				}

				return Resource_t{ id };
				//*/
				return Resource_t{ NULL_RESOURCE };
			}



			bool valid(const Resource_t& hResource) const {
				if (!hResource) return 0;

				auto res = m_Resources.contains(hResource);
				return res;
			}


			const Node* try_get_node(const Resource_t& hResource) const {
				auto it = m_Resources.find(hResource);

				if (it == m_Resources.end())
					return nullptr;

				return &it->second;
			}

			Node* try_get_node(const Resource_t& hResource) {
				auto it = m_Resources.find(hResource);

				if (it == m_Resources.end())
					return nullptr;

				return &it->second;
			}

			const T* try_get(const Resource_t& hResource) const {
				auto it = m_Resources.find(hResource);

				if (it == m_Resources.end())
					return nullptr;

				return &it->second.value;
			}

			T* try_get(const Resource_t& hResource) {
				auto it = m_Resources.find(hResource);

				if (it == m_Resources.end())
					return nullptr;
				// I'm assuming it's always valid here
				QK_ASSERT(valid(hResource));
				return &it->second.value;
			}




		

			str_view label(const Resource_t& hResource) const {
				const Node* node = try_get_node(hResource);
				if (!node) return {};
				return node->label;
			}

			// refcount management
			int refcount(const Resource_t& hResource) const {
				const Node* node = try_get_node(hResource);
				if (!node) return -1;
				return node->refcount;
			}

			bool acquire(const Resource_t& hResource) {
				Node* node = try_get_node(hResource);

				if (!node)
					return false;

				node->refcount++;
				return true;
			}

			bool release(const Resource_t& hResource) {

				Node* node = try_get_node(hResource);

				if (!node)
					return false;
				if (node->refcount == 0)
				{
					QK_ASSERT(false);
					return false;
				}
				node->refcount--;
				return true;

			}

			bool destroy(const Resource_t& hResource)
			{
				Node* node = try_get_node(hResource);

				if (!node)
					return false;

				if (node->refcount != 0)
					return false;

				m_Resources.erase(hResource);
				return true;
			}

	};






}