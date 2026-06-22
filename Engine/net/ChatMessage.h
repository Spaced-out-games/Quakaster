/// **************************************** QUAKASTER ENGINE **************************************** 
/// net/ChatMessage.h 
/// Purpose: Serves as a basic example of a packet type compatable with Quakaster's networking library
/// **************************************************************************************************

#pragma once
#include "PacketQueue.h"



// Basic message structure
struct ChatMessage {
	char msg[256]{};
};




namespace qk::net {


	// In order to send a ChatMessage over wire, we need to define the channel for chat messages. Here, it's 0 but it can be anything so long as it's unique.

	template <>
	struct Channel_traits<ChatMessage> {
		static constexpr u8 channel = 0;
	};
}