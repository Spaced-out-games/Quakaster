#pragma once 
#include <core.h>
#include "KeyValues.h"
#include <variant>
#include <emhash/hash_table7.hpp>

// runtime-optimized KeyValues map. Useful for level scripting

namespace qk {
	using KeyValuesRT_SupportedTypes = KeyValuesPrimitives;

	using KeyValuesRTValueStorage_t = UnpackTypeList<std::variant, KeyValuesRT_SupportedTypes>::type;

	struct KeyValuesRT: public emhash7::HashMap<u64, KeyValuesRTValueStorage_t> {
		using emhash7::HashMap<u64, KeyValuesRTValueStorage_t>::HashMap;
		
		// any array types in the kvs will cause an error!
		KeyValuesRT(const KeyValues&);

	};





}

