#pragma once
#include "SSBO.h"
#include "GL/glew.h"

namespace qk::gfx {
	
	size_t SSBO_impl::capacity() {
		return m_Capacity;
	}
	void SSBO_impl::size() {
		return m_Size;
	}
	void* SSBO_impl::read_impl() {
		if (m_Mode.readable) return m_Data;
		return nullptr;
	}
	void* SSBO_impl::write_impl() {
		if (m_Mode.writable) return m_Data;
		return nullptr;
	}

	bool SSBO_impl::set_readable(bool new_state) {
		if (m_Mode.read_locked) return false;
		m_Mode.readable = new_state;
		return true;
	}
	bool SSBO_impl::set_writable(bool new_state) {
		if (m_Mode.write_locked) return false;
		m_Mode.writable = new_state;
		return true;
	}

	void SSBO_impl::destroy() {
		glDeleteBuffers(&m_Handle);
	}

	void SSBO_impl::push_back_impl(size_t elem_size, const void* data) {

	}

	void SSBO_impl::upload_impl(const void* data, size_t size_bytes, unsigned int usage) {

	}

	void SSBO_impl::reserve_impl(size_t new_capacity, size_t elem_size) {

	}

	bool SSBO_impl::bind_impl() {
		return 0;
	}

	qk::ResourceLock<SSBO_impl> SSBO_impl::write_lock() {


	}

	void SSBO_impl::write_unlock(qk::ResourceLock<SSBO_impl>&& lock);

	qk::ResourceLock<SSBO_impl> SSBO_impl::read_lock();
	void SSBO_impl::read_unlock(qk::ResourceLock<SSBO_impl>&& lock);

	

}