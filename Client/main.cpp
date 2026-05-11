#include <iostream>
#include "Handle.h"
#include <platform\Quakaster.h>
#include "core.h"
#include "platform/Application.h"
#include "entrypoint.h"
#include <memory>
#include "platform/Window.h"
#include "gfx_rhi.h"
#include "VertexBufferLayoutDesc.h"
#include "VertexArray.h"
#include "gui_rhi.h"
#include "gui_widget_rhi.h"
#include "io/Event.h"
#include "Result.h"
#include "Texture.h"
#include "media/Image.h"
#include "gui/GUIWidget.h"
#include "gui/GUILayer.h"
#include "io/LayerStack.h"
#include "io/EventQueue.h"



#define TEST_IMAGE_PATH "C:/Users/devin/Desktop/Screenshots/Screenshot (54).png"
//#include "vendor/stb_image.h"

using namespace qk::gfx;


struct cl_app : qk::Application {
	qk::Result<int, float> test;
	qk::Window window;
	qk::Event ev;
	qk::EventQueue queue;
	rhi::VertexBufferLayoutDesc layoutDesc;
	rhi::HVertexArray vao;
	rhi::HTexture hTex;
	qk::LayerStack layers;


	glm::vec3 some_vec;

	void init(int argc, char** argv) override {
		using rhi::InputRate;
		using rhi::ScalarType;
		using rhi::VertexAttribute;
		layers.attach_queue(&queue);
		qk::startup();
		window.init(qk::Window::Size{ 480, 480 }, "Quakaster Demo", nullptr);
		window.set_event_queue(&queue);

		rhi::init(window.getProcAddress_pfn());

		//qk::media::Image img;
		//img.reload(TEST_IMAGE_PATH);
		//hTex = rhi::Texture_ctor();
		//rhi::Texture_bind(hTex, rhi::TextureType::TEX_2D);
		//rhi::Texture_upload_2D(hTex, img.data(), img.width(), img.height(), img.channels());

		auto& gui = layers.emplace_layer<qk::gfx::gui::GUILayer>(window.handle());

		auto& widget = gui.emplace<qk::gfx::gui::Vec3EditWidget>();
		widget.target = &some_vec;
		rhi::clear(0, 0, 0, 0, rhi::ClearChannels::COLOR | rhi::ClearChannels::DEPTH);

	}

	void run() override {
		window.swap_buffers();
		rhi::clear();

		window.pollEvents();
		layers.propagate_events();
		rhi::gui::begin_frame(0.016);

		layers.render();


		//rhi::gui::demo();
		//rhi::gui::image(hTex, { 2000, 2000 }, {0,0}, {1, 1});
		
		rhi::gui::end_frame();   // optional depending on your wrapper
		rhi::gui::render();

		std::cout << queue.size() << '\n';

		set_status(window.should_close());

	}

	void shutdown() override {
		rhi::gui::shutdown();
	}
};



std::unique_ptr<qk::Application> qk::create_application(int argc, char** argv) {
	return std::make_unique<cl_app>();
}





