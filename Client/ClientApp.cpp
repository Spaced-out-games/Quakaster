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


class ClientApp : public qk::Application {

	Endpoint ep;

	str in;
	ChatMessage msg;

	Connection* pServer = nullptr;

	void init(int argc, char** argv) override {

		ep.init(nullptr, 0);
		ep.on_connect = [](Endpoint& ep, Connection* pConnection) {
			spdlog::info("Conected to server!");
		};
		// connect to localhost::7777
		
		while (!ep.connect("localhost", 7777)) {

		}

		spdlog::info("Exitted Connection Loop");

		

		//map.push(ChatMessage{ "Hello World" });

	}

	void run() override {
		ep.flush();
		if (ep.m_Connections.size() == 1)
			pServer = ep.m_Connections.at(0).get();

		std::cin >> in;

		strcpy_s(msg.msg, 256, in.c_str());

		if(pServer)
			pServer->send(msg);




	}

	void shutdown() override {
	}

};

std::unique_ptr<qk::Application> qk::create_application(int argc, char** argv) {
	return std::make_unique<ClientApp>();
}