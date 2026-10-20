#pragma once
#include <vector>
#include <core.h>
#include <bit>
namespace qk {

	// TODO: Decompose

	enum class EPartitionRequestType: u8 {
		SWAP,
		SET
	};

	template <class T>
	struct PartitionRequest {

		union {

			struct {
				u32 offset_a;
				u32 offset_b;
			} swap;

			struct {
				T payload;
				u32 offset;
			} set;

		};

		EPartitionRequestType type;



	};

	



	template <class T>
	class Partition {
		u32 m_Size = 0;
		u32 m_Capacity = 0;
		u32 m_NextSize = 0;
		u32 m_NextCapacity = 0;

		std::vector <PartitionRequest<T>> m_Queue;
		
		public:

			static constexpr u32 NULL_OFFSET = -1u;

			Partition();

			u32 size() const;
			u32 capacity() const;

			u32 new_size() const;
			u32 new_capacity() const;

			void reserve(u32 size);

			void queue_swap(u32 offset_a, u32 offset_b);

			void queue_set(const T& payload, u32 offset);


			// Inserts the payload and returns the index it was inserted into
			u32 queue_insert(const T& payload);

			// Removes the element via swap and pop semantics. Returns back() index before it's performed. Returns NULL_OFFSET on failure
			[[nodiscard]] u32 remove(u32 offset);

			//void remove(u32 offset);

			size_t queue_size() const;

			u32 pop_back();

			void flush(T* pBase);



	};


}

#include "Partition.inl"