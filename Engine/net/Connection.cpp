#include "Connection.h"


namespace qk::net {


    bool Connection::connect(ENetHost* pHost, const char* address, u16 port, u8 num_channels, u32 data) {
        if (m_pPeer) return false;

        ENetAddress addr{};
        addr.port = port;
        enet_address_set_host(&addr, address);

        m_pPeer = enet_host_connect(pHost, &addr, num_channels, data);

        if (!m_pPeer)
            return false;

        return true;
    }

    void Connection::disconnect()
    {
        if (!m_pPeer) return;

        enet_peer_disconnect(m_pPeer, 0);


        m_pPeer = nullptr;
    }

    bool Connection::connected() const
    {
        return m_pPeer &&
            m_pPeer->state == ENET_PEER_STATE_CONNECTED;
    }

    void Connection::force_reset()
    {
        if (!m_pPeer) return;

        enet_peer_reset(m_pPeer);
        m_pPeer = nullptr;
    }

    Connection::~Connection()
    {
        disconnect();
    }

}