#include "DebugMessenger.h"
#include <spdlog/spdlog.h>
#include <string>

namespace qk::gfx {
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

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

	void DebugMessenger::destroy(const Device& paired_device) {
		DestroyDebugUtilsMessengerEXT(paired_device.instance(), m_Messenger, nullptr);
	}

	DebugMessenger::DebugMessenger(VkDebugUtilsMessengerEXT messenger) { m_Messenger = messenger; }

	DebugMessenger AttachDebugMessenger(const Device& target, EDebugMessageSeverity severity,
		EDebugMessageType type, void* custom) {
		VkInstance instance = target.instance();
		assert(instance);

		// todo: make sure layers are actually valid

		// ---- Load function pointer ----
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");


		if (!func) {
			spdlog::error("Failed to load vkCreateDebugUtilsMessengerEXT");
			return {};
		}


		VkDebugUtilsMessengerCreateInfoEXT createInfo = {
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,

			.messageSeverity = static_cast<VkDebugUtilsMessageSeverityFlagsEXT>(severity),

			.messageType = static_cast<VkDebugUtilsMessageTypeFlagsEXT>(type),

			.pfnUserCallback = debugCallback,

			.pUserData = custom,
		};




		VkDebugUtilsMessengerEXT messenger_hdl = VK_NULL_HANDLE;

		VkResult result = func(instance, &createInfo, nullptr, &messenger_hdl);

		if (result != VK_SUCCESS) {
			spdlog::error("Failed to create debug messenger");
			return {};
		}
		spdlog::info("debug manager created at {}", (void*)messenger_hdl);

		return DebugMessenger{ messenger_hdl };

	}
}