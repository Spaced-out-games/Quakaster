#pragma once
#include <fstream>
#include <cstdint>
#include <stdexcept>

using entity_type_t = uint16_t;


// Pass this into an entity's de/serialize method by reference, and it will read/write and advance the offset appropriately
struct serializer_context {
	// Pointer to the file being read, or the file created on the heap
	uint8_t* data;

	// Offset from the pointer (in bytes) the current context is at.
	uint32_t offset;

	// The total size of the buffer pointed to.
	uint32_t size;

};

struct block_header {

	// The type of entity contiguously stored in this block
	entity_type_t type;
	
	// Used to calculate the size of the block
	uint8_t exponent;

	size_t size() {
		return 2 << exponent;
	}
};