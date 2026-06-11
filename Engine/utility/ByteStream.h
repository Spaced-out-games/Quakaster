#pragma once
#include "core.h"



namespace qk::utility {

	



	class ByteStream {
		std::vector<u8> m_Stream;

		public:

			void push_back(const void* pData, size_t size);
			void push_front(const void* pData, size_t size);


			void pop_back(void* pData, size_t size);
			void pop_front(void* pData, size_t size);


	};

}