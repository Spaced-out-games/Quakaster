#pragma once
#include "core.h"
#include <vector>

namespace qk::utility {
    class ByteStream;
}

namespace qk {

    template<class T>
    bool serialize(utility::ByteStream&, const T&);

    template<class T>
    bool deserialize(utility::ByteStream&, T&);
}

namespace qk::utility {

    class ByteStream {
        std::vector<u8> m_buffer;
        size_t m_offset = 0;

    protected:
        bool write_impl(const void* data, size_t size);
        const u8* read_impl(size_t size);

        template<class T>
        friend bool qk::serialize(ByteStream&, const T&);

        template<class T>
        friend bool qk::deserialize(ByteStream&, T&);

    public:
        void begin_read();
        const u8* data() const;
        size_t size() const;
        void clear();

    };

}