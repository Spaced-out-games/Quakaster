// setup
#include <platform/Quakaster.h>
#include "entrypoint.h"
#include "core.h"


// standard library
//#include <filesystem>
#include <iostream>
#include <vector>
#include <memory>

// platform
#include "platform/Application.h"
#include "platform/Window.h"
#include "net/NetEndpoint.h"
#include <spdlog/spdlog.h>
#include "net/Connection.h"



class ClientApp : public qk::Application, qk::net::NetEndpoint {
	ChatMessage outgoing;


	bool try_connect(const char* pIPAddress) {
		if (!connect(pIPAddress)) {
			spdlog::error("[CLIENT]: Failed to connect\n");
			return 0;
		}
		return 1;
	}


	void init(int argc, char** argv) override {
		
		if (!NetEndpoint::init(nullptr)) {
			spdlog::error("[CLIENT]: Failed to create client\n");
			setStatus(2);
			return;
		}

		if (!try_connect("127.0.0.1")) {
			setStatus(3);
			return;
		}


		


	}

	void run() override {
		std::cout << "> ";
		std::cin.getline(outgoing.msg, sizeof(outgoing));
		if (std::strcmp(outgoing, "quit") == 0)
			setStatus(1);
		send_impl(outgoing, 256, 0);

		auto evt = receive_impl(&outgoing, 256, 100);

		if (evt.type == qk::net::STATUS_RECEIVE) {
			spdlog::info("[CLIENT]: {}", outgoing.msg);
		}
	}

	void shutdown() override {
		disconnect();
	}

};

std::unique_ptr<qk::Application> qk::create_application(int argc, char** argv) {
	return std::make_unique<ClientApp>();
}