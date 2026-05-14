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

#include "gfx/Material.h"
#include "gfx/Mesh.h"

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
	-0.5,-0.5,
	0.5,-0.5,
	0.0,0.5
};


struct cl_app : qk::Application {
	
	// core needs
	qk::Window window;
	qk::EventQueue queue;
	qk::LayerStack layers;

	glm::vec3 color = {1,1,1};
	glm::vec3 position = {};

	// rhi types
	//rhi::HVertexArray hVertexArray;
	//rhi::HVertexBuffer hVertexBuffer;
	rhi::HVertexBufferLayout hLayout;
	rhi::VertexBufferLayoutDesc LayoutDesc;

	//Material mat_flat;
	Mesh mesh_triangle;


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
		gui.emplace<qk::gfx::gui::Vec3EditWidget>("position", &position, -5.0f, 5.0f);
	}


	void init(int argc, char** argv) override {

		attach_queues();
		render_init();
		attach_layers();

		// create the vertex buffer layout
		LayoutDesc.push(rhi::f32, 2, 0);
		hLayout = rhi::VertexBufferLayout_ctor(LayoutDesc);



		mesh_triangle.init(points, hLayout, demoVertShaderSource, demoFragShaderSource, rhi::StaticDraw);
		mesh_triangle.push_uniform<glm::vec3>("u_Color", &color);
		mesh_triangle.push_uniform<glm::vec3>("u_Position", &position);
		


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
		mesh_triangle.bind();
		mesh_triangle.update_uniforms();
		rhi::DrawArrays(mesh_triangle.m_hVertexArray, rhi::Triangle, 0, 3);
		layers.render();


		// presents offscreen image
		window.swap_buffers();
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





