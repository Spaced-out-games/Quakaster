#include "ClientApplication.h"
#include "spdlog/spdlog.h"
#include <iostream>

void ClientApplication::init(int argc, char** argv) {
	std::cout << qk::length(line);

	// set up event queues...
	//Application::init(argc, argv);
	window.set_event_queue(&evt_queue);
	evt_layerstack.attach_queue(&evt_queue);


	qk::startup();
	window.init({480, 480}, "Quakaster Client");
}

void ClientApplication::run() {
	window.pollEvents();
	set_status(window.should_close());
}

void ClientApplication::close() {
	window.destroy();
	qk::shutdown();
}