#pragma once
#include "core.h"
#include <vector>




namespace qk::utility {

	
    



    class ByteStream
    {
        std::vector<u8> m_buffer;
        size_t m_offset = 0;

        protected:
        // write implementation
        bool write_impl(const void* data, size_t size);

        // Returns a pointer to the place in vector memory by which we can start deserializing
        const u8* read_impl(size_t size);

        friend void serialize(ByteStream&, const void*, size_t);
        friend bool deserialize(ByteStream&, void*, size_t);




    public:


        void begin_read();

        const u8* data() const;
        size_t size() const;
        void clear();







    };
    /*
    template <class T>
    ByteStream& operator<<(ByteStream& bs, const T& value) {
        bs.push(value);
        return bs;
    }

    template <class T>
    ByteStream& operator>>(ByteStream& bs, T& value) {
        bs.pop(value);
        return bs;
    }
    */
}