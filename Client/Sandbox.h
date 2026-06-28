#pragma once

// setup
#include <platform/Quakaster.h>
#include "entrypoint.h"
#include "core.h"

// standard library
//#include <filesystem>
#include <iostream>
#include <vector>
#include <memory>

// logging
#include <spdlog/spdlog.h>


// io
#include "io/EventQueue.h"
#include "io/LayerStack.h"

// platform
#include "platform/Application.h"
#include "platform/Window.h"

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// media
#include "media/Image.h"
#include "media/json.h"

// GUI
#include "gui/GUILayer.h"

// RHI
#include "HVertexArray.h"
#include "HVertexLayout.h"
#include "HCommandBuffer.h"
#include "gfx_rhi.h"
#include "Resource.h"
struct Vertex {
	float x, y, z;
};

struct CameraData {
	glm::mat4 view = { 1.0f };
	glm::mat4 proj = { 1.0f };

};


static inline CameraData g_Camera;

class Sandbox : public qk::Application {
	qk::Window m_Window;
	qk::EventQueue m_EventQueue;
	qk::LayerStack m_Layers;



	// A little hack to make assets/ readable in debug mode, where Client.exe is deep in the woods
	inline void add_symlinks() {

		namespace fs = std::filesystem;
		#ifdef _DEBUG
		// NOTE: Requires admin priviledges. 
		if (!fs::exists("assets")) fs::create_directory_symlink("../../../../assets/", "assets/");


		#endif
	}

	// Can be called before or after render_init()
	inline void attach_event_queues() {
		m_Layers.attachQueue(&m_EventQueue);
		m_Window.setEventQueue(&m_EventQueue);
	}

	// initializes the renderer and window.
	inline void render_init() {
		qk::startup(); // calls glfwInit
		m_Window.init(qk::Window::Size{ 1920, 1080 }, "Quakaster Demo", nullptr);
	}

	// this has to be called last because some m_Layers might depend on m_Window Resources
	inline void attach_layers() {
		auto& gui = m_Layers.emplace_layer<qk::gfx::gui::GUILayer>(m_Window.handle());

	}

	inline void init_subsystems() {
		add_symlinks();
		attach_event_queues();
		render_init();
		attach_layers();
	}


	void init(int argc, char** argv) override;

	void run() override;

	void shutdown() override;

};

std::unique_ptr<qk::Application> qk::create_application(int argc, char** argv);