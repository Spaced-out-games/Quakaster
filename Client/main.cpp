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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// media
#include "media/Image.h"

#include "gfx_rhi.h"
#include "gui_rhi.h"
#include "gui_widget_rhi.h"
#include "gui/GUILayer.h"
#include "gui/GUIWidget.h"

#include "HVertexArray.h"
#include "HVertexLayout.h"
#include "Draw.h"
#include "HCommandBuffer.h"


#include "HMaterial.h"



using namespace qk::gfx;





struct Vertex {
	float x, y, z;
};


std::vector<Vertex> g_Vertices = {
		{  0.0f,  0.5f, 0.0f },
		{ -0.5f, -0.5f, 0.0f },
		{  0.5f, -0.5f, 0.0f }
};




const char* vertexSource = R"(
#version 330 core

layout(location = 0) in vec3 aPos;

layout(std140) uniform CameraData {
	mat4 view;
	mat4 proj;

};

void main()
{
    gl_Position = view * vec4(aPos, 1.0);
}
)";

const char* fragmentSource = R"(
#version 330 core





out vec4 FragColor;

void main()
{
    FragColor = vec4(
        1.0,
        0.2,
        0.3,
        1.0);
}
)";


struct CameraData {
	glm::mat4 view = { 1.0f };
	glm::mat4 proj = { 1.0f };

};



struct DemoClientApplication : qk::Application {
	

	// core needs
	qk::Window m_Window;
	qk::EventQueue m_EventQueue;
	qk::LayerStack m_Layers;

	CameraData data;

	rhi::HVertexArray vao;

	rhi::HMaterial material;

	rhi::HCommandBuffer CmdBuf;

	rhi::HUniformBuffer uniforms;

	rhi::VertexLayout_createinfo layout_createinfo;

	rhi::CommandBuffer_createinfo CmdBufDesc;


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

	}



	void init(int argc, char** argv) override {



		attach_event_queues();
		render_init();
		attach_layers();



		// Vertex Array
		vao = rhi::VertexArray_ctor();
		rhi::VertexArray_bind(vao);

		// ============================================================
		// Vertex Buffer
		// ============================================================

		rhi::HVertexBuffer vbo = rhi::VertexBuffer_ctor(rhi::StaticDraw, g_Vertices);
		rhi::VertexBuffer_bind(vbo);

		// ============================================================
		// Vertex Layout
		// ============================================================
		rhi::VertexBinding binding = layout_createinfo.pushBinding(rhi::PerVertex, sizeof(Vertex));
		layout_createinfo.push(binding, 0, 0, rhi::layout::vec3f);
		rhi::HVertexLayout hLayout = rhi::VertexLayout_ctor(layout_createinfo);
		rhi::VertexLayout_apply(hLayout);

		

		uniforms = rhi::UniformBuffer_ctor();
		rhi::UniformBuffer_upload(uniforms, &data, 1, sizeof(data));



		rhi::DescriptorSet_createinfo DescriptorSet_info;

		DescriptorSet_info.push_back(rhi::UniformBuffer_desc{
			.hBuf = uniforms,
			.id = "CameraData",
			.binding = 0,
		});





		material = rhi::Material_ctor(rhi::Material_createinfo{ vertexSource, fragmentSource, DescriptorSet_info });



		

		// finally, the command buffer
		CmdBufDesc.push_preexecute(rhi::ClearCommand_desc{ 0.0,0.0,0.0,0.0, rhi::Color });
		CmdBufDesc.push(rhi::DrawArrays_desc{ material, vao, rhi::Triangle, 0, 3 });


		CmdBuf = rhi::CommandBuffer_ctor(CmdBufDesc);

	}
	void run() override {

		// populates event m_EventQueue
		m_Window.pollEvents();
		// flushes it
		m_Layers.propagate_events();

		rhi::UniformBuffer_upload(uniforms, &data, 1, sizeof(data));
		data.view = glm::translate(data.view, glm::vec3{ 0.00001f,0,0 });

		rhi::CommandBuffer_execute(CmdBuf);
		m_Layers.render();

		//rhi::Material_dtor(material);

		//rhi::VertexArray_dtor(vao);

		// presents offscreen image
		m_Window.swapBuffers();


		// updates when told to by the m_Window.
		setStatus(m_Window.shouldClose());

	}

	void shutdown() override {
	}
};



std::unique_ptr<qk::Application> qk::create_application(int argc, char** argv) {
	return std::make_unique<DemoClientApplication>();
}





