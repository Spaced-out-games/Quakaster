#include "ClientApplication.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <nvrhi/nvrhi.h>
#include <nvrhi/vulkan.h>
#include <boost/pfr.hpp>


// todo: create a deletion stack so that cleanup() is more or less empty
//LifetimeManager

void print_device_desc(const nvrhi::vulkan::DeviceDesc& desc)
{
    std::cout << "Instance extensions:\n";

    for (size_t i = 0; i < desc.numInstanceExtensions; i++)
    {
        std::cout << "  " << desc.instanceExtensions[i] << "\n";
    }

    std::cout << "Device extensions:\n";

    for (size_t i = 0; i < desc.numDeviceExtensions; i++)
    {
        std::cout << "  " << desc.deviceExtensions[i] << "\n";
    }

    std::cout << "Queues:\n";
    std::cout << "  graphicsQueueIndex: " << desc.graphicsQueueIndex << "\n";
    std::cout << "  transferQueueIndex: " << desc.transferQueueIndex << "\n";
    std::cout << "  computeQueueIndex: " << desc.computeQueueIndex << "\n";
}



void ClientApplication::init(int argc, char** argv) {
    using enum qk::gfx::EDebugMessageSeverity;
    using enum qk::gfx::EDebugMessageType;
    using enum qk::gfx::EDeviceFeatureSet;
    using enum qk::gfx::EQueueFeatureSet;

    qk::startup();

    

    // build the device description
    qk::gfx::DeviceDescBuilder builder;
    auto desc = builder
        .appinfo("Quakaster Demo", 0)
        .version(QK_MAKE_API_VERSION(0,1,1,0))
        .device_features(NO_FEATURES)
        .queue_features(GRAPHICS | TRANSFER | COMPUTE)
        .add_glfw_extensions()
        .build();

    std::cout << "device Description: \n";
    print_device_desc(desc);
    

    // attach a debug messenger.
    //gpu_messenger = gfx::AttachDebugMessenger(gpu, ALL_SEVERITIES, ALL_TYPES);

    // resource management
    dtor_queue.push([this]() { qk::shutdown(); window.destroy(); });
    dtor_queue.push([this]() { gpu_messenger.destroy(gpu); });

    // set up event queues...
    window.set_event_queue(&evt_queue);
    evt_layerstack.attach_queue(&evt_queue);

    window.init({ 480, 480 }, "Quakaster Client");
    //window.make_context_current();
}

void ClientApplication::run() {
	window.pollEvents();
	//window.swap_buffers();
	set_status(window.should_close());
}

void ClientApplication::shutdown() {
	dtor_queue.flush();
}