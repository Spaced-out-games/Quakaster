// setup
#include <platform/Quakaster.h>
#include "entrypoint.h"
#include "core.h"

// standard library
#include <memory>

// platform
#include "platform/Application.h"
#include "platform/Window.h"

// io
#include "io/EventQueue.h"
#include "io/LayerStack.h"

// gui
#include "gui_rhi.h"
#include "gui/GUILayer.h"
#include "gui/GUIWidget.h"
//#include "gui_widget_rhi.h"





#define TEST_IMAGE_PATH "C:/Users/devin/Desktop/Screenshots/Screenshot (54).png"

using namespace qk::gfx;


struct cl_app : qk::Application {
	qk::Window window;
	qk::EventQueue queue;
	qk::LayerStack layers;
	glm::vec3 demo_vec;

	// Can be called before or after render_init()
	void attach_queues() {
		layers.attach_queue(&queue);
		window.set_event_queue(&queue);
	}



	void render_init() {
		qk::startup();
		window.init(qk::Window::Size{ 480, 480 }, "Quakaster Demo", nullptr);
		rhi::init(window.getProcAddress_pfn());
	}

	// this has to be called last because some layers depend on window handles
	void attach_layers() {
		auto& gui = layers.emplace_layer<qk::gfx::gui::GUILayer>(window.handle());

		auto& vec3e = gui.emplace<qk::gfx::gui::Vec3EditWidget>();

		vec3e.target = &demo_vec;

	}


	void init(int argc, char** argv) override {


		attach_queues();

		render_init();

		attach_layers();


		rhi::clear(0, 0, 0, 0, rhi::ClearChannels::COLOR | rhi::ClearChannels::DEPTH);
		
	}

	void run() override {

		// populates event queue
		window.pollEvents();
		// flushes it
		layers.propagate_events();

		// flushes offscreen image
		rhi::clear();

		// renders to offscreen image
		layers.render();

		// presents offscreen image
		window.swap_buffers();

		// updates when told to by the window.
		set_status(window.should_close());

	}

	void shutdown() override {
		rhi::gui::shutdown();
	}
};



std::unique_ptr<qk::Application> qk::create_application(int argc, char** argv) {
	return std::make_unique<cl_app>();
}





