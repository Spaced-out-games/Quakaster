#include "vk_builder.h"
#include <nvrhi/vulkan.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

namespace qk::gfx::vk {

	struct BuildInfo {
		VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
		VkInstance				 Instance		= VK_NULL_HANDLE;
		VkPhysicalDevice		 PhysicalDevice = VK_NULL_HANDLE;
		VkDevice				 Device			= VK_NULL_HANDLE;
	};


	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		std::string msg_type_str = "";


		if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
			msg_type_str += "[VALIDATION]";
		if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
			msg_type_str += "[PERF]";

		const char* msg_type = msg_type_str.c_str();

		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			spdlog::trace("[VK]{}: {}", msg_type, pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			spdlog::info("[VK]{}: {}", msg_type, pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			spdlog::warn("[VK]{}: {}", msg_type, pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			spdlog::error("[VK]{}: {}", msg_type, pCallbackData->pMessage);
			break;
		default:
			spdlog::debug("[VK]{}: {}", msg_type, pCallbackData->pMessage);
			break;
		}


		return VK_FALSE;
	}

	bool Builder::check_extension_support() const {

		// auto - populate mandatory extensions
		uint32_t num_extensions = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &num_extensions, nullptr);
		std::vector<VkExtensionProperties> extensions(num_extensions);
		vkEnumerateInstanceExtensionProperties(nullptr, &num_extensions, extensions.data());

		for (const char* ext : m_Extensions) {
			if (std::find_if(extensions.begin(), extensions.end(), [&](VkExtensionProperties& e) { return strcmp(e.extensionName, ext) == 0; }) == extensions.end()) {
				spdlog::warn("Extension '{}' not supported by Vulkan.", ext);
				return false;
			}
		}



		return true;

	}

	bool Builder::check_layer_support() const {

		bool result = true;
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());


		for (const char* layerName : m_Layers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				spdlog::warn("Validation layer '{}' not supported by Vulkan", layerName);
				result = false;
			}
		}

		return result;
	}

	void Builder::add_debug_manager(VkInstance instance) const {
		VkResult code = VK_SUCCESS;
		VkDebugUtilsMessengerCreateInfoEXT messenger_createInfo{

			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,

			.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,

			.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,

			.pfnUserCallback = debugCallback,

		};

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			code = func(instance, &messenger_createInfo, nullptr, &m_Info->debugMessenger);
		}
		else {
			code = VK_ERROR_EXTENSION_NOT_PRESENT;
		}

		if (code != VK_SUCCESS) {
			spdlog::error("[VK] failed to set up debug messenger!");
			throw std::runtime_error("[VK] failed to set up debug messenger!");
		}

	}





	Builder& Builder::set_device_heuristic(DeviceHeuristic heuristic) {
		m_DeviceHeuristic = heuristic;
	}


	Builder::Builder() {
		m_Info = new BuildInfo();
		uint32_t glfw_extension_count = 0;
		const char** glfw_extensions;
		glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

		m_Extensions.insert(
			m_Extensions.end(),
			glfw_extensions,
			glfw_extensions + glfw_extension_count
		);

		#ifdef QK_GFX_USE_VALIDATION_LAYERS
			m_Layers.emplace_back("VK_LAYER_KHRONOS_validation");
			m_Extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		#endif

	}

	Builder::~Builder() {
		delete m_Info;
	}


	Builder& Builder::set_application_details(const std::string& app_name, uint32_t app_version) {
		m_ApplicationName = app_name;
		m_ApplicationVersion = app_version;
		return *this;
	}

	Builder& Builder::set_API_version(uint32_t api_version) {
		m_APIVersion = api_version;
		return *this;
	}

	Builder& Builder::add_extension(const char* extension) {
		m_Extensions.push_back(extension);
		return *this;
	}

	Builder& Builder::add_layer(const char* layer) {
		m_Layers.push_back(layer);
		return *this;
	}

	Builder& Builder::devices(std::vector<uint32_t>& device_map) {

		// get all the devices
		uint32_t num_devices = 0;
		vkEnumeratePhysicalDevices(m_Info->Instance, &num_devices, nullptr);
		if (num_devices == 0) {
			spdlog::error("found no GPUs to support Vulkan on this machine");
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		}
		std::vector<VkPhysicalDevice> candidates(num_devices);
		vkEnumeratePhysicalDevices(m_Info->Instance, &num_devices, candidates.data());

		// filter obviously unsuitable

		return *this;
	}

	Builder& Builder::use_device(uint32_t device_id) {

		m_DeviceID = device_id;

		return *this;
	}

	Builder& Builder::build_instance() {
		// -------------------------------------------- INSTANCE CREATION -------------------------------------------- //
		VkResult code = VK_SUCCESS;

		VkApplicationInfo appinfo = {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pApplicationName = m_ApplicationName.c_str(),
			.applicationVersion = m_ApplicationVersion,
			.pEngineName = "Quakaster Engine",
			.engineVersion = QK_VERSION,
			.apiVersion = m_APIVersion == 0 ? VK_API_VERSION_1_0 : m_APIVersion
		};

		VkInstanceCreateInfo createinfo = {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = m_Flags,
			.pApplicationInfo = &appinfo,
			.enabledLayerCount = (uint32_t)m_Layers.size(),
			.ppEnabledLayerNames = m_Layers.data(),
			.enabledExtensionCount = (uint32_t)m_Extensions.size(),
			.ppEnabledExtensionNames = m_Extensions.data(),
		};
		#ifndef QK_GFX_USE_VALIDATION_LAYERS
		createinfo.ppEnablesExtensionNames = nullptr;
		#endif
		if (vkCreateInstance(&createinfo, nullptr, &m_Info->Instance) != VK_SUCCESS) {
			spdlog::error("vkCreateInstance() failure!");
			throw std::runtime_error("vkCreateInstance() failure!");
		}

		add_debug_manager(&m_Info->Instance);
	}






	nvrhi::DeviceHandle Builder::build_device() const {



		// --------------------------------------------- DEBUG MESSENGER --------------------------------------------- //
		add_debug_manager(&m_Info->Instance);




		return nullptr;
	}



}