#pragma once
#include "core.h"
#include <vector>


namespace qk::utility {
    // Vector of bytes.
    class ByteStream {
        std::vector<u8> m_buffer;
        size_t m_offset = 0;


    public:

        bool write(const void* data, size_t size);
        const u8* read(size_t size);

        void begin_read();
        const u8* data() const;
        size_t size() const;
        void clear();

    };

}