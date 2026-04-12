#include "DeviceBuilder.h"
#include <nvrhi/vulkan.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

namespace qk::gfx {


	



	// Make a Vulkan instance without the boilerplate
	VkInstance make_vk_instance(const std::string& app_name, uint32_t app_version, uint32_t api_version,
		const std::vector<const char*>& extensions, const std::vector<const char*>& layers, uint32_t flags) {
		VkInstance instance;


		VkApplicationInfo appinfo = {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pApplicationName = app_name.c_str(),
			.applicationVersion = app_version,
			.pEngineName = "Quakaster Engine",
			.engineVersion = QK_VERSION,
			.apiVersion = api_version == 0 ? VK_API_VERSION_1_0 : api_version
		};

		VkInstanceCreateInfo createinfo = {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = flags,
			.pApplicationInfo = &appinfo,
			.enabledLayerCount = (uint32_t)layers.size(),
			.ppEnabledLayerNames = layers.empty() ? nullptr : layers.data(),
			.enabledExtensionCount = (uint32_t)extensions.size(),
			.ppEnabledExtensionNames = extensions.empty() ? nullptr : extensions.data(),
		};
		VkResult result = vkCreateInstance(&createinfo, nullptr, &instance);

		if (result != VK_SUCCESS) {
			spdlog::error("vkCreateInstance() failure!");
			throw std::runtime_error("vkCreateInstance() failure!");
		}
		else {
			spdlog::info("vkCreateInstance() success!");
		}


		return instance;

	}


	

	// checks for extension support
	bool check_extension_support(const std::vector<const char*>& queried_extensions) {

		// auto - populate mandatory extensions
		uint32_t num_extensions = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &num_extensions, nullptr);
		std::vector<VkExtensionProperties> extensions(num_extensions);
		vkEnumerateInstanceExtensionProperties(nullptr, &num_extensions, extensions.data());

		for (const char* ext : queried_extensions) {
			if (std::find_if(extensions.begin(), extensions.end(), [&](VkExtensionProperties& e) { return strcmp(e.extensionName, ext) == 0; }) == extensions.end()) {
				spdlog::warn("Extension '{}' not supported by Vulkan.", ext);
				return false;
			}
		}



		return true;

	}



	// --------------------------------------------  DEVICE CREATION  -------------------------------------------- //

	DeviceDescBuilder::DeviceDescBuilder() {
		uint32_t glfw_extension_count = 0;
		const char** glfw_extensions;
		glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

		m_Extensions.insert(
			m_Extensions.end(),
			glfw_extensions,
			glfw_extensions + glfw_extension_count
		);

		#ifdef QK_GFX_USE_VALIDATION_LAYERS
		m_Extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		#endif

	}

	DeviceDescBuilder& DeviceDescBuilder::set_device_scoring_fn(DeviceScoring_fn func) {
		m_DeviceScoring_fn = func;
		return *this;
	}

	DeviceDescBuilder& DeviceDescBuilder::appinfo(const std::string& app_name, uint32_t app_version) {
		m_ApplicationName = app_name;
		m_ApplicationVersion = app_version;
		return *this;
	}

	DeviceDescBuilder& DeviceDescBuilder::version(uint32_t api_version) {
		m_APIVersion = api_version;
		return *this;
	}

	DeviceDescBuilder& DeviceDescBuilder::add_extension(const char* extension) {
		m_Extensions.push_back(extension);
		return *this;
	}



	Device DeviceDescBuilder::build() {
		Device device;
		

		device.m_Instance = make_vk_instance(m_ApplicationName, m_ApplicationVersion, m_APIVersion, m_Extensions, m_Layers, m_Flags);

		// get vulkan devices
		
		
		// placeholder
		return device;

	}












}