/// **************************************** QUAKASTER ENGINE **************************************** 
/// utility/ByteStream.h 
/// Purpose: Serialization
/// **************************************************************************************************
#pragma once
#include "core.h"
#include <vector>


namespace qk::utility {
    // Vector of bytes.
    class ByteStream {
        std::vector<u8> m_buffer;
        size_t m_offset = 0;


    public:

        // Appends pData to the end of the vector
        bool write(const void* pData, size_t size);

        // Returns a pointer to the next `size` bytes in the stream. Returns nullptr if there is not `size` bytes left
        const u8* read(size_t size);

        // Resets the byte offset
        void begin_read();
        // Pointer to the front of the underlying vector
        const u8* data() const;

        // The size of the buffer in bytes
        size_t size() const;

        // Clears the buffer
        void clear();

    };

}