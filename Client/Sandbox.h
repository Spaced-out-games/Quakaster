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
#include "kernel/ComponentLoader.h"
#include "kernel/EntityLoader.h"
#include "kernel/SceneLoader.h"
#include "fnv1a.h"

#include "gfx/Renderer.h"
#include "math/geometric/Vec3.h"

#include "math/geometric/Mat4.h"
#include "Handle.h"
#include <Partition.h>
#include "gfx/InstancePool.h"
#include "utility/Profiler.h"
#include "gui/ProfilerWidget.h"

using namespace qk::gfx::rhi;

using qk::operator""_sh;

#include <World/World.h>


using namespace qk;
qk::EntityLoader loader;





void deserialize_transform(entt::registry&, entt::entity, const qk::KeyValues& args) {
	const auto get_vec3 = [&](const char* key) -> std::optional<glm::vec3> {
		if (!args.contains(key))
			return std::nullopt;

		const auto value = args[key].as<std::vector<f32>>();

		if (!value || value->size() != 3)
			return std::nullopt;

		return glm::vec3{(*value)[0],(*value)[1],(*value)[2]};
	};

	if (const auto position = get_vec3("CTransform.position"))
		spdlog::info(
			"CTransform.position: {}, {}, {}",
			position->x, position->y, position->z
		);

	if (const auto rotation = get_vec3("CTransform.rotation"))
		spdlog::info(
			"CTransform.rotation: {}, {}, {}",
			rotation->x, rotation->y, rotation->z
		);

	if (const auto scale = get_vec3("CTransform.scale"))
		spdlog::info(
			"CTransform.scale: {}, {}, {}",
			scale->x, scale->y, scale->z
		);
}




const qk::ComponentLoader component_loaders[] = {
	ComponentLoader {
		"CTransform",
		std::array<str_view,3> {
			"CTransform.position",
			"CTransform.rotation",
			"CTransform.scale"
		}, deserialize_transform
		
	}
};

template <typename T>
concept Streamable = requires(std::ostream & os, const T & value) {
	{ os << value } -> std::same_as<std::ostream&>;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
	os << '[';

	for (size_t i = 0; i < vec.size(); ++i) {
		if (i) os << ", ";
		os << vec[i];
	}

	return os << ']';
}


KernelCallResult sys_print_transform(Kernel&, VDrive* pVDrive, const std::vector<f32>& position, const std::vector<f32>& rotation, const std::vector<f32>& scale) {

	std::cout << position;
	std::cout << rotation;
	std::cout << scale;

	return KernelCallResult{ KERNEL_STATUS_SUCCESS, KeyValues() };
}

KernelCallResult sys_print_kvs(Kernel&, VDrive* pVDrive, const KeyValues& kvs) {

	// Positional arguments
	for (size_t i = 0; i < kvs.size(); ++i) {
		std::cout << '[' << i << "]: ";

		std::visit([](const auto& item) {
			using T = std::remove_cvref_t<decltype(item)>;

		if constexpr (Streamable<T>)
			std::cout << item;
		else
			std::cout << "<unstreamable>";
		}, kvs[i]);

		std::cout << '\n';
	}

	return KernelCallResult{ KERNEL_STATUS_SUCCESS, KeyValues() };

}



void loader_demo(entt::registry& registry, entt::entity target, Kernel& kernel) {
	qk::EntityLoader loader;
	loader.push_loaders(component_loaders);

	qk::KeyValues args;
	args.insert("CTransform.position", std::vector<f32>{ 0.0f, 0.0f, 0.0f });
	args.insert("CTransform.rotation", std::vector<f32>{ 0.0f, 90.0f, 0.0f });
	args.insert("CTransform.scale", std::vector<f32>{ 1.0f, 1.0f, 1.0f });
	//args.insert("GARBAGE", "this is a garbage value");

	VDrive* pVDrive = new VDrive(qk::KernelPermissions::ENTRY_CREATE);

	//pVDrive->m_pSyscalls["print_transform"]  
	auto a = qk::kernel_trampoline<&sys_print_transform, std::vector<f32>, std::vector<f32>, std::vector<f32>>;

	a(kernel, nullptr, args);
	//if (pVDrive->syscall(kernel, "print_transform", args).status != KERNEL_STATUS_SUCCESS) {
	//	std::cout << "BAD ARGUMENTS";
	//}
	



	loader.execute(registry, target, args);
}


class Sandbox : public qk::Application {
	// application
	qk::Window m_Window;
	qk::EventQueue m_EventQueue;
	qk::LayerStack m_Layers;

	qk::Kernel m_Kernel;

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

	// testing
	gfx::MeshInstance instance;
	gfx::MeshDefinitionID          m_MeshID;
	std::vector<gfx::MeshInstance> m_Live;
	float t = 0.0f;

	// profiling
	qk::utility::ProfilerRegistry m_Profilers;
	qk::utility::Profiler* m_pFrame = nullptr;

	// A little hack to make assets/ readable in debug mode, where Client.exe is deep in the woods
	inline void add_symlinks() {

		namespace fs = std::filesystem;
		#if (!QK_DEBUG)
		spdlog::warn("Adding symlink in release mode!");
		#endif

		// NOTE: Requires admin priviledges. 
		if (!fs::exists("assets"))
			fs::create_directory_symlink("../../../../assets/", "assets/");


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
		m_pFrame = &m_Profilers.create("frame");


		
		auto& gui = m_Layers.emplace_layer<qk::gfx::gui::GUILayer>(m_Window.handle());
		gui.emplace<qk::gfx::ProfilerWidget>(&m_Profilers, "Frame profiler");
		//m_pFrame.
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