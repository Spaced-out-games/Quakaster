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
#include "media/LoadPipeline.h"

// GUI
#include "gui/GUILayer.h"

// RHI
#include "HVertexLayout.h"
#include "HCommandBuffer.h"
#include "HPipeline.h"
#include "HBuffer.h"
#include "gfx_rhi.h"
#include "Resource.h"

#include "spacial/CSpatialHashMap.h"


using namespace qk::gfx::rhi;
struct Vertex {
	float x, y, z;
};

#include "math/geometric/Vec3.h"
using namespace qk;

void spatial_demo() {
	SpatialHashMap3D<int, 4> map;
	
	

	map.insert(Vec3{ 0,0,0 }, 0);
	map.insert(Vec3{ 0,0,1 }, 1);
	map.insert(Vec3{ 0,0,2 }, 2);
	map.insert(Vec3{ 0,0,3 }, 3);

	map.insert(Vec3{ 0,0,4 }, 4);

	const auto& cell1 = map.values_at(Vec3i{ 0,0,0 });
	const auto& cell2 = map.values_at(Vec3i{ 0,0,1 });

	
	assert(cell1.contains(0) && cell1.contains(1) && cell1.contains(2) && cell1.contains(3) && !cell1.contains(4));

	assert(!cell2.contains(0) && !cell2.contains(1) && !cell2.contains(2) && !cell2.contains(3) && cell2.contains(4));
	//*/

	map.insert(Vec3{ 3.999f,0,0 }, 0);
	map.insert(Vec3{ 4.000f,0,0 }, 1);

	assert(map.contains(Vec3i{ 0,0,0 }, 0));
	assert(map.contains(Vec3i{ 1,0,0 }, 1));

	map.remove(Vec3{ 0,0,2 }, 2);

	assert(!map.contains(Vec3i{ 0,0,0 }, 2));

	map.remove(Vec3{ 0,0,0 }, 0);
	map.remove(Vec3{ 0,0,1 }, 1);
	map.remove(Vec3{ 0,0,3 }, 3);
	assert(map.contains(Vec3i{ 0,0,1 }, 4));


	
	map.for_each_cell([](const Vec3i& cell, const SpatialHashMap3D<int, 4>::Bucket_t& bucket) {

		std::cout << cell << ": " << bucket.size() << "item(s)" << '\n';
	});

}


class Sandbox : public qk::Application {
	qk::Window m_Window;
	qk::EventQueue m_EventQueue;
	qk::LayerStack m_Layers;

	std::unordered_map<str, Pipeline_recipe> pipelines;
	std::unordered_map<str, DescriptorSet_recipe> sets;

	HCommandBuffer hCmdBuf = nullptr; 
	HDescriptorSet hGlobals = nullptr;
	HDescriptorSet hMaterial = nullptr;
	HPipeline hPipeline = nullptr;
	HTexture hTex = nullptr;
	HUniformBuffer hUniformBuffer = nullptr;
	HVertexBuffer hVertexBuffer = nullptr;
	HVertexBuffer hInstanceBuffer = nullptr;

	// A little hack to make assets/ readable in debug mode, where Client.exe is deep in the woods
	inline void add_symlinks() {

		namespace fs = std::filesystem;
		#ifdef _DEBUG
		// NOTE: Requires admin priviledges. 
		if (!fs::exists("assets"))
			fs::create_directory_symlink("../../../../assets/", "assets/");


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
		qk::gfx::rhi::init(m_Window);

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