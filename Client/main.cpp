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


// graphics
#include "gfx/Material.h"
#include "gfx/Mesh.h"

// media
#include "media/Image.h"



#define TEST_IMAGE_PATH "C:/Users/devin/Desktop/Screenshots/Screenshot (54).png"

using namespace qk::gfx;

static const char* demoVertShaderSource = R"(

#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 iOffset;
layout(location = 2) in vec3 iColor;

out vec3 fColor;
out vec2 aUV;
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
	vec2 worldPos = (aPos + iOffset);
	gl_Position = vec4(worldPos, 0.0, 1.0);
	fColor = iColor;
	aUV = aPos * 0.5 + 0.5;
}

)";

static const char* demoFragShaderSource = R"(

#version 330 core
in vec2 aUV;

uniform sampler2D u_Texture;

in vec3 fColor;
out vec4 FragColor;

void main()
{
	//vec4 tex = vec4(1,1,1, 1);
    vec4 tex = texture(u_Texture, aUV); // TEMP placeholder UV
    FragColor = vec4(tex.rgb, tex.a); // fColor + 
}

)";


struct InstanceData {
	glm::vec2 pos;
	glm::vec3 color;
};


std::vector<float> g_Points = {
	-0.5,-0.5,
	0.5,-0.5,
	0.0,0.5
};

std::vector<float> g_Instances = {
	-0.5,-0.5,		1,0,0,
	-0.5,0.5,		0,1,0,
	0.5,-0.5,		0,0,1
};

struct DemoClientApplication : qk::Application {
	

	// core needs
	qk::Window m_Window;
	qk::EventQueue m_EventQueue;
	qk::LayerStack m_Layers;

	glm::vec3 m_Color = {1,1,1};
	glm::vec3 m_Position = {};

	VertexBufferLayout m_Layout;
	Mesh m_DemoMesh;
	Material m_DemoMat;
	int slot = 0;

	// Can be called before or after render_init()
	void attach_event_queues() {
		m_Layers.attachQueue(&m_EventQueue);
		m_Window.setEventQueue(&m_EventQueue);
	}

	void render_init() {
		qk::startup();
		m_Window.init(qk::Window::Size{ 1920, 1080 }, "Quakaster Demo", nullptr);
		rhi::init(qk::getProcAddress());
	}

	// this has to be called last because some m_Layers might depend on m_Window Resources
	void attach_layers() {
		auto& gui = m_Layers.emplace_layer<qk::gfx::gui::GUILayer>(m_Window.handle());

		gui.emplace<qk::gfx::gui::Vec3EditWidget>("Position", &m_Position, -5.0f, 5.0f);
	}


	void init(int argc, char** argv) override {

		VertexBufferLayout vert_layout;
		rhi::VertexBufferLayoutDesc vert_layout_desc;
		vert_layout_desc.push(rhi::f32, 2, 0, InputRate::Vertex, ScalarMultiplicity::Scalar, 0);
		vert_layout.init(vert_layout_desc);

		VertexBufferLayout instance_layout;
		rhi::VertexBufferLayoutDesc instance_layout_desc;
		instance_layout_desc.push(rhi::f32, 2, 0, InputRate::Instance, ScalarMultiplicity::Scalar, 1);
		instance_layout_desc.push(rhi::f32, 3, 0, InputRate::Instance, ScalarMultiplicity::Scalar, 2);
		instance_layout.init(instance_layout_desc);

		media::Image img;
		media::load_image(img, TEST_IMAGE_PATH);



		attach_event_queues();
		render_init();
		attach_layers();


		Texture test_texture;


		test_texture.init();
		test_texture.bind(0);
		test_texture.upload_2D(img.data(), img.width(), img.height(), img.channels());


		m_DemoMesh.init();
		m_DemoMesh.pushVertexBuffer(std::span<float>(g_Points), rhi::StaticDraw, vert_layout);
		m_DemoMesh.pushVertexBuffer(std::span<float>(g_Instances), rhi::StaticDraw, instance_layout);
		m_DemoMat.init(demoVertShaderSource, demoFragShaderSource);
		//m_DemoMat.pushUniform<int>("u_Texture", &slot);


		m_DemoMat.pushTexture("u_Texture", std::move(test_texture));


		// set clear settings.
		rhi::clear(0, 0, 0, 0, rhi::ClearChannels::Color | rhi::ClearChannels::Stencil);

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
		m_DemoMat.bind();
		m_DemoMat.update();
		m_Layers.render();

		rhi::DrawArraysInstanced(m_DemoMesh.ref(), Triangle, 0, 3, 3);


		// presents offscreen image
		m_Window.swapBuffers();


		// updates when told to by the m_Window.
		setStatus(m_Window.shouldClose());

	}

	void shutdown() override {
		rhi::gui::shutdown();
	}
};



std::unique_ptr<qk::Application> qk::create_application(int argc, char** argv) {
	return std::make_unique<DemoClientApplication>();
}





