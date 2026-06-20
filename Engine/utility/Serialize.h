#pragma once
#include "ByteStream.h"
#include <cstring>

namespace qk {

    // Pushes bytes to a vector
    template<class T>
    bool serialize(utility::ByteStream& bs, const T& value)
    {
        return bs.write(&value, sizeof(T));
    }
    // Pops bytes from a vector
    template<class T>
    bool deserialize(utility::ByteStream& bs, T& value)
    {
        const u8* ptr = bs.read(sizeof(T));

        if (!ptr)
            return false;

        memcpy(&value, ptr, sizeof(T));
        return true;
    }

}