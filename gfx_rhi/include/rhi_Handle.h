#include <unordered_map>
#include <type_traits>
#include <fnv1a.h>
#include <core.h>
#define RHI_USE_VALIDATION_LAYERS 1

namespace qk::gfx::rhi {

	constexpr u64 NULL_HANDLE = UINT64_MAX;

	class Handle {
		
		u64 hResource = NULL_HANDLE;
		public:
			operator u64() const { return hResource; }

			Handle(u64 id): hResource(id) {}
			Handle() = default;

			Handle(qk::string_hash label): hResource(label.hash) {}

			Handle& operator=(u64) = delete;
			Handle& operator=(const qk::string_hash&) = delete;

			bool operator==(const Handle&) const = default;

			explicit operator bool() const {
				return hResource != NULL_HANDLE;
			}

	};
	template <class T, class Handle_t>
	class ResourceCache {
		static_assert(std::is_base_of_v<Handle, Handle_t>, "THandle must derive from Handle");

		struct Node {
			T			value{};
			str label;
			u32 refcount = 0;
		};


		std::unordered_map<u64, Node> m_Resources;

		public:

			Handle_t insert(str_view label, const T& value) {
				u64 id = (u64)qk::hash_fnv1a(label);
				auto [it, inserted] =
					m_Resources.try_emplace(id, Node{
						.value = value,
						.label = label,
						.refcount = 1
				});

				if (!inserted)
				{
					// hash collision detection
					if (it->second.label != label) {
						QK_ASSERT(false);
					}
					return Handle_t{ NULL_HANDLE };
				}

				return Handle_t{ id };

			}



			bool is_valid(const Handle_t& hResource) const {
				if (!hResource) return 0;
				return m_Resources.contains(hResource);
			}


			const Node* try_get_node(const Handle_t& hResource) const {
				auto it = m_Resources.find(hResource);

				if (it == m_Resources.end())
					return nullptr;

				return &it->second;
			}

			Node* try_get_node(const Handle_t& hResource) {
				auto it = m_Resources.find(hResource);

				if (it == m_Resources.end())
					return nullptr;

				return &it->second;
			}

			const T* try_get(const Handle_t& hResource) const {
				auto it = m_Resources.find(hResource);

				if (it == m_Resources.end())
					return nullptr;

				return &it->second.value;
			}

			T* try_get(const Handle_t& hResource) {
				auto it = m_Resources.find(hResource);

				if (it == m_Resources.end())
					return nullptr;

				return &it->second.value;
			}




		

			str_view label(const Handle_t& hResource) const {
				const Node* node = try_get_node(hResource);
				if (!node) return {};
				return node->label;
			}

			// refcount management
			int refcount(const Handle_t& hResource) const {
				const Node* node = try_get_node(hResource);
				if (!node) return -1;
				return node->refcount;
			}

			bool acquire(const Handle_t& hResource) {
				Node* node = try_get_node(hResource);

				if (!node)
					return false;

				node->refcount++;
				return true;
			}

			bool release(const Handle_t& hResource) {

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

			bool destroy(const Handle_t& hResource)
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