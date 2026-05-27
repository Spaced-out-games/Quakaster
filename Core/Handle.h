/// **************************************** QUAKASTER ENGINE **************************************** 
/// Core/Result.h
/// Purpose: Defines an opaque Resource type that smuggles Resources past encapsulation borders.
///          Also defines an opaque reference to these Resources
/// **************************************************************************************************
#pragma once
#include <vector>
#include <stdexcept>

namespace qk {

	struct Handle {
		uint32_t index;
		uint32_t generation;
	};

	template <typename To, typename From>
	To Handle_cast(const From& from) {

		static_assert(std::is_base_of_v<Handle, To>,
			"To must derive from Handle");

		static_assert(std::is_base_of_v<Handle, From>,
			"From must derive from Handle");

		return To{
			from.index,
			from.generation
		};
	}



	template <class T, class THandle>
	class HandleResourceRegistry {
		static_assert(std::is_base_of_v<Handle, THandle>,
			"THandle must derive from Handle");

	private:

		struct Node {
			T value{};
			uint32_t generation = 1;
			bool alive = false;
		};

		std::vector<Node> m_Storage;
		std::vector<uint32_t> m_Freelist;

	public:

		THandle insert(const T& value) {

			// Reuse dead slot
			if (!m_Freelist.empty()) {

				uint32_t index = m_Freelist.back();
				m_Freelist.pop_back();

				Node& node = m_Storage[index];

				node.value = value;
				node.alive = true;

				return THandle{
					index,
					node.generation
				};
			}

			// Allocate new slot
			uint32_t index = (uint32_t)(m_Storage.size());

			Node node;
			node.value = value;
			node.alive = true;

			m_Storage.push_back(std::move(node));

			return THandle{
				index,
				m_Storage[index].generation
			};
		}

		bool destroy(const THandle& handle) {

			if (handle.index >= m_Storage.size()) {
				return 0;
			}

			Node& node = m_Storage[handle.index];

			if (!node.alive ||
				node.generation != handle.generation) {
				return 0;
			}

			node.alive = false;
			++node.generation;

			m_Freelist.push_back(handle.index);
			return 1;
		}

		T* try_get(const THandle& handle) {

			if (handle.index >= m_Storage.size()) {
				return nullptr;
			}

			Node& node = m_Storage[handle.index];

			if (!node.alive ||
				node.generation != handle.generation) {
				return nullptr;
			}

			return &node.value;
		}

		T& get(const THandle& handle) {
			return *try_get(handle);
		}

		const T* try_get(const THandle& handle) const {

			if (handle.index >= m_Storage.size()) {
				return nullptr;
			}

			const Node& node = m_Storage[handle.index];

			if (!node.alive ||
				node.generation != handle.generation) {
				return nullptr;
			}

			return &node.value;
		}

		bool valid(const THandle& handle) const {
			if (handle.index >= m_Storage.size()) return false;


			const Node& node = m_Storage[handle.index];

			return node.alive &&
				node.generation == handle.generation;
		}
	};


} // namespace qk