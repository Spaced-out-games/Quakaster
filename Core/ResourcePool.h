#pragma once
#include <stdint.h>
#include "Resource.h"
#include <unordered_map>
#include <cassert>
#include <utility>
#include <string>

namespace qk {
	template <class T>
	using dtor_fn = void(*)(T&);

	template <class tag_t, void(*dtor)(Resource<tag_t>&)>
	class ResourcePool {


		using handle_t	= Resource<tag_t>;
		using ref_t		= Ref<tag_t>;

		
		std::unordered_map<uint64_t, handle_t> m_Storage;

		public:
			bool insert(uint64_t id, handle_t&& handle) {
				return m_Storage.emplace(id, std::move(handle)).second;
			}

			bool remove(uint64_t id) {
				auto it = m_Storage.find(id);
				if (it == m_Storage.end()) return 0;
				dtor(m_Storage[id]);
			}

			ref_t get(uint64_t id)
			{
				auto it = m_Storage.find(id);

				if (it == m_Storage.end())
					return handle_t{};

				return it->second.ref();
			}

			bool has(uint64_t id) {
				return m_Storage.find(id) == m_Storage.end(id);
			}

	};


}