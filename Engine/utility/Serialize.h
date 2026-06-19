#pragma once
#include "ByteStream.h"
#include <cstring>

namespace qk {

    template<class T>
    bool serialize(utility::ByteStream& bs, const T& value)
    {
        return bs.write_impl(&value, sizeof(T));
    }

    template<class T>
    bool deserialize(utility::ByteStream& bs, T& value)
    {
        const u8* ptr = bs.read_impl(sizeof(T));

        if (!ptr)
            return false;

        memcpy(&value, ptr, sizeof(T));
        return true;
    }

}