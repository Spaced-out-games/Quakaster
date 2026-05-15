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


std::vector<float> g_Points = {
	-0.5,-0.5,
	0.5,-0.5,
	0.0,0.5
};


struct DemoClientApplication : qk::Application {
	
	// core needs
	qk::Window m_Window;
	qk::EventQueue m_EventQueue;
	qk::LayerStack m_Layers;

	glm::vec3 m_Color = {1,1,1};
	glm::vec3 m_Position = {};


	rhi::HVertexBufferLayout hLayout;
	rhi::VertexBufferLayoutDesc LayoutDesc;

	Mesh m_DemoMesh;


	// Can be called before or after render_init()
	void attach_m_EventQueues() {
		m_Layers.attach_queue(&m_EventQueue);
		m_Window.set_event_queue(&m_EventQueue);
	}

	void render_init() {
		qk::startup();
		m_Window.init(qk::Window::Size{ 1920, 1080 }, "Quakaster Demo", nullptr);
		rhi::init(m_Window.getProcAddress_pfn());
	}

	// this has to be called last because some m_Layers might depend on m_Window handles
	void attach_layers() {
		auto& gui = m_Layers.emplace_layer<qk::gfx::gui::GUILayer>(m_Window.handle());

		gui.emplace<qk::gfx::gui::Vec3EditWidget>("Color", &m_Color, 0.0f, 1.0f);
		gui.emplace<qk::gfx::gui::Vec3EditWidget>("Position", &m_Position, -5.0f, 5.0f);
	}


	void init(int argc, char** argv) override {

		attach_m_EventQueues();
		render_init();
		attach_layers();

		// create the vertex buffer layout
		LayoutDesc.push(rhi::f32, 2, 0);
		hLayout = rhi::VertexBufferLayout_ctor(LayoutDesc);



		m_DemoMesh.init(g_Points, hLayout, demoVertShaderSource, demoFragShaderSource, rhi::StaticDraw);
		m_DemoMesh.pushUniform<glm::vec3>("u_Color", &m_Color);
		m_DemoMesh.pushUniform<glm::vec3>("u_Position", &m_Position);
		


		// set clear settings.
		rhi::clear(0, 0, 0, 0, rhi::ClearChannels::COLOR | rhi::ClearChannels::DEPTH);

	}

	void run() override {

		// populates event m_EventQueue
		m_Window.pollEvents();
		// flushes it
		m_Layers.propagate_events();

		// flushes offscreen image
		rhi::clear();

		// renders to offscreen image
		m_DemoMesh.bind();
		m_DemoMesh.updateUniforms();
		rhi::DrawArrays(m_DemoMesh.m_hVertexArray, rhi::Triangle, 0, 3);
		m_Layers.render();


		// presents offscreen image
		m_Window.swap_buffers();


		// updates when told to by the m_Window.
		set_status(m_Window.should_close());

	}

	void shutdown() override {
		rhi::gui::shutdown();
	}
};



std::unique_ptr<qk::Application> qk::create_application(int argc, char** argv) {
	return std::make_unique<DemoClientApplication>();
}





