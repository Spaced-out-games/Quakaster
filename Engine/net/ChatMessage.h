#pragma once
#include "PacketQueue.h"



// Basic packet example

struct ChatMessage {
	char msg[256]{};
};




namespace qk::net {

	template <>
	struct Channel_traits<ChatMessage> {
		static constexpr u8 channel = 3;
	};
}