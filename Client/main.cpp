// setup
#include <platform/Quakaster.h>
#include "entrypoint.h"
#include "core.h"

// standard library
#include <memory>
#include <iostream>

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

// rhi
#include "gfx_draw.h"
#include "HVertexArray.h"
#include "VertexBufferLayoutDesc.h"
#include "HVertexBufferLayout.h"
#include "HVertexBuffer.h"
#include "HShader.h"
#include "HShaderProgram.h"
#include "HDescriptorSet.h"

#include "media/json.h"

#define TEST_IMAGE_PATH "C:/Users/devin/Desktop/Screenshots/Screenshot (54).png"

using namespace qk::gfx;

static const char* demoVertShaderSource = R"(

#version 330 core

layout(location = 0) in vec2 aPos;
uniform vec3 u_Position;

mat2 rotation2D(float a)
{
    float s = sin(a);
    float c = cos(a);

    return mat2(
         c, -s,
         s,  c
    );
}



void main()
{
    gl_Position = vec4((aPos * rotation2D(u_Position.z))+ u_Position.xy , 0.0, 1.0);
}

)";


static const char* demoFragShaderSource = R"(

#version 330 core
uniform vec3 u_Color;
out vec4 FragColor;

void main()
{
    FragColor = vec4(u_Color, 1.0);
}

)";


std::vector<float> points = {
	0,0,
	0,1,
	1,0
};



struct cl_app : qk::Application {
	
	// core needs
	qk::Window window;
	qk::EventQueue queue;
	qk::LayerStack layers;

	glm::vec3 color = {};
	glm::vec3 position = {};

	// rhi types
	rhi::HVertexArray hVertexArray;
	rhi::HVertexBuffer hVertexBuffer;
	rhi::HVertexBufferLayout hLayout;
	rhi::VertexBufferLayoutDesc LayoutDesc;

	rhi::HShader hVertexShader;
	rhi::HShader hFragmentShader;

	rhi::HShaderProgram hShaderProgram;
	rhi::HDescriptorSet hDescriptorSet;
	rhi::HShaderUniform hColor;
	rhi::HShaderUniform hPosition;

	
	// Can be called before or after render_init()
	void attach_queues() {
		layers.attach_queue(&queue);
		window.set_event_queue(&queue);
	}

	void render_init() {
		qk::startup();
		window.init(qk::Window::Size{ 1920, 1080 }, "Quakaster Demo", nullptr);
		rhi::init(window.getProcAddress_pfn());
	}

	// this has to be called last because some layers might depend on window handles
	void attach_layers() {
		auto& gui = layers.emplace_layer<qk::gfx::gui::GUILayer>(window.handle());

		gui.emplace<qk::gfx::gui::Vec3EditWidget>("color", &color, 0.0f, 1.0f);
		gui.emplace<qk::gfx::gui::Vec3EditWidget>("position", &position, -1.0f, 1.0f);
	}


	void init(int argc, char** argv) override {

		attach_queues();
		render_init();
		attach_layers();
		
		// Construct the vertex array and bind
		hVertexArray = rhi::VertexArray_ctor();
		rhi::VertexArray_bind(hVertexArray);
		// create the vertex buffer layout
		LayoutDesc.push(rhi::VertexAttribute{ rhi::f32, 2, 0 });
		hLayout = rhi::VertexBufferLayout_ctor(LayoutDesc);

		hVertexBuffer = rhi::VertexBuffer_ctor();
		rhi::VertexBuffer_upload(hVertexBuffer, points.data(), points.size(), sizeof(float), rhi::StaticDraw);

		// apply the layout
		rhi::VertexArray_applyLayout(hVertexArray, hVertexBuffer, hLayout);


		






		// Fragment shader
		auto RFrag = rhi::Shader_ctor(rhi::ShaderType::Fragment);

		if (RFrag.has_value()) {
			hFragmentShader = std::move(RFrag.value());
			rhi::Shader_upload(hFragmentShader, demoFragShaderSource);
		} else std::cout << RFrag.error();

		// Vertex Shader
		auto RVert = rhi::Shader_ctor(rhi::ShaderType::Vertex);

		if (RVert.has_value()) {
			hVertexShader = std::move(RVert.value());
			rhi::Shader_upload(hVertexShader, demoVertShaderSource);
		} else std::cout << RVert.error();


		// Shader Program
		auto RProgram = rhi::ShaderProgram_ctor(hFragmentShader, hVertexShader);

		if (RProgram.has_value())
			hShaderProgram = std::move(RProgram.value());
		else
			std::cout << RProgram.error();

		auto RColor = rhi::ShaderProgram_getUniform(hShaderProgram, "u_Color");

		if (RColor.has_value())
			hColor = RColor.value();

		auto RPosition = rhi::ShaderProgram_getUniform(hShaderProgram, "u_Position");

		if (RPosition.has_value())
			hPosition = RPosition.value();


		



		hDescriptorSet = rhi::DescriptorSet_ctor();
		

		rhi::DescriptorSet_push(hDescriptorSet, hColor, rhi::f32, 3, rhi::ScalarMultiplicity::Vector, &color);
		rhi::DescriptorSet_push(hDescriptorSet, hPosition, rhi::f32, 3, rhi::ScalarMultiplicity::Vector, &position);

		rhi::DescriptorSet_update(hShaderProgram, hDescriptorSet, hColor);
		rhi::DescriptorSet_update(hShaderProgram, hDescriptorSet, hPosition);




		// set clear settings.
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
		rhi::ShaderProgram_bind(hShaderProgram);
		rhi::DrawArrays(hVertexArray, rhi::Triangle, 0, 3);

		// presents offscreen image
		window.swap_buffers();
		rhi::DescriptorSet_update(hShaderProgram, hDescriptorSet);
		//rhi::DescriptorSet_update(hShaderProgram, hDescriptorSet, hPosition);


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





