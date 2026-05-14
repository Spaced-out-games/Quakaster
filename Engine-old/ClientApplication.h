#include <core/core/Quakaster.h>
#include <core/core/Application.h>
#include <core/core/Window.h>

#include <core/utility/DestructionQueue.h>
#include <core/io/EventQueue.h>
#include <core/io/LayerStack.h>
#include <core/io/Layer.h>

#include <entrypoint.h>
#include <core.h>


#include <gfx/vk/DeviceBuilder.h>
#include <gfx/vk/DebugMessenger.h>
#include <gfx/vk/Device.h>
using namespace qk;

struct ClientApplication : core::Application {

	core::Window			window;
	io::LayerStack			evt_layerstack;
	io::EventQueue			evt_queue;
	util::DestructionQueue	dtor_queue;
	gfx::Device				gpu;
	gfx::DebugMessenger		gpu_messenger;
	void init(int argc, char** argv) override;

	void run() override;

	void shutdown() override;


};

std::unique_ptr<core::Application> qk::create_application(int argc, char** argv) {
	return std::make_unique<ClientApplication>();
}