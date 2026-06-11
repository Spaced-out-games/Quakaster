#include <entrypoint.h>
#include "net/NetEndpoint.h"
#include "platform/Application.h"
#include <spdlog/spdlog.h>

class ServerApp : public qk::Application, public qk::net::NetEndpoint
{

    ChatMessage incoming;
public:

    void init(int argc, char** argv) override
    {
        ENetAddress addr{};
        addr.host = ENET_HOST_ANY;
        addr.port = 7777;

        if (!NetEndpoint::init(&addr))
        {
            spdlog::error("[SERVER]: Failed to start server");
            setStatus(1);
            return;
        }

        spdlog::info("[SERVER]: Listening on port 7777");
    }

    void run() override
    {
        auto evt = receive_impl(&incoming, sizeof(incoming), 100);

        switch (evt.type)
        {
        case qk::net::STATUS_CONNECT:
            spdlog::info("[SERVER]: Client connected");

            send_impl("[SERVER]: Welcome!", 256, 0);
            break;

        case qk::net::STATUS_DISCONNECT:
            spdlog::info("[SERVER]: Client disconnected");
            break;

        case qk::net::STATUS_RECEIVE:
        {

            spdlog::info("[CLIENT]: {}", incoming.msg);

            break;
        }

        default:
            break;
        }
    }

    void shutdown() override
    {
        disconnect();
    }
};

std::unique_ptr<qk::Application>
qk::create_application(int argc, char** argv)
{
    return std::make_unique<ServerApp>();
}