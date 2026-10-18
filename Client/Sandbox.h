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
#include <VertexLayoutDesc.h>
#include "HCommandBuffer.h"
#include "HPipeline.h"
#include "HBuffer.h"
#include "gfx_rhi.h"
#include "Resource.h"

#include "spacial/CSpatialHashMap.h"
#include "scene/ComponentLoader.h"
#include "scene/EntityLoader.h"
#include "scene/SceneLoader.h"
#include "fnv1a.h"

#include "gfx/Renderer.h"
#include "math/geometric/Vec3.h"

#include "math/geometric/Mat4.h"
#include "Handle.h"
#include <Partition.h>
#include "gfx/InstancePool.h"

using namespace qk::gfx::rhi;

using qk::operator""_sh;

using namespace qk;
qk::EntityLoader loader;

const qk::ComponentLoader component_loaders[] = {
	ComponentLoader{
		.name = "CTransform",
		.dependencies = {
			"CTransform/position"_sh,
			"CTransform/rotation"_sh,
			"CTransform/scale"_sh
		},
		.loader = [](entt::registry&, entt::entity, const EntityArgumentMap& args) {
			if (args.contains("CTransform/position"_sh)) {
				const auto& position =
					std::get<std::vector<f32>>(args.at("CTransform/position"_sh));

				spdlog::info(
					"CTransform/position: {}, {}, {}",
					position[0],
					position[1],
					position[2]
				);
			}

			if (args.contains("CTransform/rotation"_sh)) {
				const auto& rotation =
					std::get<std::vector<f32>>(args.at("CTransform/rotation"_sh));

				spdlog::info(
					"CTransform/rotation: {}, {}, {}",
					rotation[0],
					rotation[1],
					rotation[2]
				);
			}

			if (args.contains("CTransform/scale"_sh)) {
				const auto& scale =
					std::get<std::vector<f32>>(args.at("CTransform/scale"_sh));

				spdlog::info(
					"CTransform/scale: {}, {}, {}",
					scale[0],
					scale[1],
					scale[2]
				);
			}

		}	
	}


};
void loader_demo(entt::registry& registry, entt::entity target) {
	qk::EntityLoader loader;
	loader.push_loaders(component_loaders);

	EntityArgumentMap args = {
		{
			"CTransform/position"_sh,
			std::vector<f32>{ 0.0f, 0.0f, 0.0f }
		},
		{
			"CTransform/rotation"_sh,
			std::vector<f32>{ 0.0f, 90.0f, 0.0f }
		},
		{
			"CTransform/scale"_sh,
			std::vector<f32>{ 1.0f, 1.0f, 1.0f }
		},
		{
			"GARBAGE"_sh, 0
		}
	};

	loader.execute(registry, target, args);
}


class Sandbox : public qk::Application {
	// application
	qk::Window m_Window;
	qk::EventQueue m_EventQueue;
	qk::LayerStack m_Layers;

	std::unordered_map<str, Pipeline_recipe> pipelines;
	std::unordered_map<str, DescriptorSetLayout> sets;

	// renderer
	gfx::Renderer<glm::vec3, glm::mat4> r;


	// pipeline
	HPipeline hPipeline = nullptr;

	// gbuffers
	HVertexBuffer hVertexBuffer = nullptr;
	HVertexBuffer hInstanceBuffer = nullptr;

	// descriptor sets
	HDescriptorSet hTextureSet = nullptr;
	HDescriptorSet hTintSet = nullptr;

	// descriptors
	HTexture hTextureResource = nullptr;
	HUniformBuffer hTint = nullptr;

	// commandbuffers
	HCommandBuffer hCmdBuf = nullptr;




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
		m_Window.init(qk::Window::Size{ 1920 / 2, 1080 / 2 }, "Quakaster Demo", nullptr);
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