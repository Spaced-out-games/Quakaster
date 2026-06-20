// setup
#include <platform/Quakaster.h>
#include "entrypoint.h"
#include "core.h"
#include "net/ChatMessage.h"
#include "net/Endpoint.h"

// standard library
//#include <filesystem>
#include <iostream>
#include <vector>
#include <memory>

// platform
#include "platform/Application.h"
#include "platform/Window.h"
#include <spdlog/spdlog.h>

using qk::net::Endpoint;
using qk::net::Connection;


class ServerApp : public qk::Application {

	Endpoint ep;

	void init(int argc, char** argv) override {

		ep.init(nullptr, 7777);
		ep.on_connect = [](Endpoint& ep, Connection* pConnection) {
			pConnection->add_queue<ChatMessage>();
			spdlog::info("Client connected!");
		};

		ep.on_receive = [](Endpoint& ep, Connection* pConnection) {
			ChatMessage msg;

			bool status = pConnection->pop(msg);

			if(status)
				spdlog::info("[CLIENT]: {}", msg.msg);

		};

		


		//map.push(ChatMessage{ "Hello World" });

	}

	void run() override {
		ep.flush();
	}

	void shutdown() override {
	}

};

std::unique_ptr<qk::Application> qk::create_application(int argc, char** argv) {
	return std::make_unique<ServerApp>();
}