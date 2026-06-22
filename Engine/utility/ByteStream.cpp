#include "ByteStream.h"
#include <stdexcept>

namespace qk::utility {


    bool ByteStream::write(const void* pData, size_t size)
    {
        size_t old = m_buffer.size();
        m_buffer.resize(old + size);

        std::memcpy(m_buffer.data() + old, pData, size);
        return true; // for now
    }

    const u8* ByteStream::read(size_t size)
    {

        if (m_offset + size > m_buffer.size())
            return nullptr;

        const u8* result = data() + m_offset;

        m_offset += size;

        return result;
    }


    void ByteStream::begin_read() { m_offset = 0; }

    const u8* ByteStream::data() const { return m_buffer.data(); }
    size_t ByteStream::size() const { return m_buffer.size(); }

    void ByteStream::clear() {
        m_offset = 0;
        m_buffer.clear();
    }




}