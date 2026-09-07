#pragma once
#include <vector>
#include <core.h>

namespace qk {
	// NOTE: we are using u32s over size_ts for SPARSE INDICES ONLY so we can keep handles < 64 bits
	// Consider amping up generations to 32 bits so we get more mileage!

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
			size_t sparse_size() const;
			bool valid(Handle handle) const;
			bool valid(u32 sparse) const;
			void remove(Handle handle);
			void remove(u32 sparse);

			const T* try_get(Handle handle) const;


			// LOW LEVEL: does not validate generations.
			const T* try_get_sparse(u32 sparse) const;



			T* try_get(Handle handle);

			// LOW LEVEL: does not validate generations, but is gauranteed to be safe if it doesn't return nullptr
			T* try_get_sparse(u32 sparse);

			// Swaps payloads for two live sparse indices.
			// LOW LEVEL: does not validate generations.
			void swap(u32 sparse_a, u32 sparse_b);

			void swap(Handle a, Handle b);

			// helper
			void set(Handle handle, const T& value);


			struct iterator {
				HandleTable* pTable = nullptr;
				u32			current_sparse = 0;

				iterator& operator++();

				u32 operator*();

				bool operator==(const iterator& other) const;

				bool operator!=(const iterator& other) const;

			};

			iterator begin();

			iterator end();


	};





}


#include "Handle.inl"