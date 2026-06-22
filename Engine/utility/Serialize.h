/// **************************************** QUAKASTER ENGINE **************************************** 
/// utility/Serializer.h 
/// Purpose: Defines an interface for de/serialization to a ByteStream
/// **************************************************************************************************
#pragma once
#include "ByteStream.h"
#include <cstring>

namespace qk {

    // Pushes bytes to a vector
    template<class T>
    bool serialize(utility::ByteStream& bs, const T& value)
    {
        static_assert(std::is_trivially_copyable_v<T>);
        return bs.write(&value, sizeof(T));
    }
    // Pops bytes from a vector
    template<class T>
    bool deserialize(utility::ByteStream& bs, T& value)
    {
        static_assert(std::is_trivially_copyable_v<T>);
        const u8* ptr = bs.read(sizeof(T));

        if (!ptr)
            return false;

        memcpy(&value, ptr, sizeof(T));
        return true;
    }

}