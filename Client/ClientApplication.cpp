#include "ClientApplication.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <nvrhi/nvrhi.h>
#include <nvrhi/vulkan.h>

void ClientApplication::init(int argc, char** argv) {
	VkApplicationInfo app_info{};
	std::cout << "Hello, Quakaster Client!" << std::endl;
	std::cout << "VkApplicationInfo struct size: " << sizeof(app_info) << " bytes\n";
	std::cout << "DeviceHandle struct size: " << sizeof(nvrhi::DeviceHandle) << " bytes\n";
	exit(0); // temp

	// set up event queues...
	//Application::init(argc, argv);
	window.set_event_queue(&evt_queue);
	evt_layerstack.attach_queue(&evt_queue);

	qk::startup();
	window.init({480, 480}, "Quakaster Client");
	window.make_context_current();

}

void ClientApplication::run() {
	window.pollEvents();
	window.swap_buffers();
	set_status(window.should_close());
}

void ClientApplication::close() {
	window.destroy();
	qk::shutdown();
}