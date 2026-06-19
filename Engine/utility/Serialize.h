#pragma once
#include <vector>
#include "core.h"
//#include "ByteStream.h"
#include <string.h>

using namespace qk::utility;

namespace qk::utility {
	class ByteStream;
}


namespace qk {

	template <class T>
	//requires std::is_trivially_copyable_v<T>
	bool serialize(ByteStream& bs, const T& value) {
		bs.write_impl(&value, sizeof(T));
		return 1;
	}

	template <class T>
	bool deserialize(ByteStream& bs, T& value) {

		//const u8* pArchive = bs.read_impl(sizeof(T))

		//if (!pArchive) return false;

		//memcpy(&value, pArchive, sizeof(T));

		return true;

	}
}