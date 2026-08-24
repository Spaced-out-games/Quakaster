#pragma once
#include <vector>
#include <core.h>

namespace qk {


	template<class T>
	struct HandleTable {
		struct Entry {
			T payload;
			u16 generation;
			bool alive;
		};

		struct Handle {
			u32 sparse = 0;
			u16 generation = 0;
		};

		private:

			std::vector<Entry> m_Entries;
			std::vector<u32> m_Free;

		public:
			Handle insert(const T& payload);

			bool valid(Handle handle) const;

			void remove(Handle handle);

			const T* try_get(Handle handle) const;

			T* try_get(Handle handle);

			// Swaps payloads for two live sparse indices.
			// LOW LEVEL: does not validate generations.
			void swap(u32 sparse_a, u32 sparse_b);

			void swap(Handle a, Handle b);

			// helper
			void set(Handle handle, const T& value);



	};





}


#include "Handle.inl"