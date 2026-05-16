#pragma once
#include "gfx.h"
#include "../core.h"
#include <stdint.h>

namespace qk::gfx {
	
	struct FSSBOAccessMode {
		union {
			struct {
				uint8_t readable : 1;
				uint8_t writable : 1;
				uint8_t read_locked : 1;
				uint8_t write_locked : 1;
				uint8_t reserved : 4;  // fill the byte
			};
			uint8_t value;
		};
	};




	class SSBO_impl {
		protected:
			size_t m_Size = 0;
			size_t m_Capacity = 0;
			void*  m_Data = nullptr;
			gfx::Handle m_Handle = gfx::NULL_HANDLE;
			FSSBOAccessMode m_Mode = { 0 };
			unsigned int m_Counter = 0;
			
			
			void push_back_impl(size_t elem_size, const void* data);
			void upload_impl(const void* data, size_t size_bytes, unsigned int usage);
			void reserve_impl(size_t new_capacity, size_t elem_size);
			void* read_impl();
			void* write_impl();
			bool bind_impl();

			// |='s the mode, but respecting read and write locks. Returns true if actually changed
			bool set_readable(bool new_state);
			bool set_writable(bool new_state);

		public:
			size_t size();
			size_t capacity();
			static void unbind();
			void destroy();

			qk::ResourceLock<SSBO_impl> write_lock();
			void write_unlock(qk::ResourceLock<SSBO_impl>&& lock);

			qk::ResourceLock<SSBO_impl> read_lock();
			void read_unlock(qk::ResourceLock<SSBO_impl>&& lock);

	};

}