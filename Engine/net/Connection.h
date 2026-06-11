#pragma once
#include <stdexcept>
#include "core.h"
#include <enet/enet.h>


namespace qk::net {

    class Connection {



        ENetPeer* m_pPeer = nullptr;

        public:
            // --- connect (non-blocking, no event polling here) ---
            bool connect(
                ENetHost* pHost,
                const char* address = "127.0.0.1",
                u16 port = 7777,
                u8 num_channels = 1,
                u32 data = 0
            );

            // --- disconnect (graceful, still event-driven) ---
            void disconnect();

            // --- state query ---
            bool connected() const;

            // --- safety reset (used by Endpoint when needed) ---
            void force_reset();

            ~Connection();
    };

} // namespace qk::net