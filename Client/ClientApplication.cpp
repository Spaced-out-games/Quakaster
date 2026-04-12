#include "ClientApplication.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <nvrhi/nvrhi.h>
#include <nvrhi/vulkan.h>

// todo: create a deletion stack so that cleanup() is more or less empty
//LifetimeManager


void ClientApplication::init(int argc, char** argv) {
	using enum qk::gfx::EDebugMessageSeverity;
	using enum qk::gfx::EDebugMessageType;



	// build the device
	qk::gfx::DeviceDescBuilder builder;
	gpu = builder.appinfo("Quakaster Demo", 0)
					.version(VK_API_VERSION_1_1)
					// in a complete DeviceBuilder we'd do more operations here
					.build();

	// attach a debug messenger.
	gpu_messenger = gfx::AttachDebugMessenger(gpu, ALL_SEVERITIES, ALL_TYPES);

	// resource management
	dtor_queue.push([this]() { window.destroy(); qk::shutdown(); });
	dtor_queue.push([this]() { gpu_messenger.destroy(gpu);  });


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
	//window.swap_buffers();
	set_status(window.should_close());
}

void ClientApplication::shutdown() {
	dtor_queue.flush();
}