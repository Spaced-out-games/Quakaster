#pragma once
#include <vulkan/vk_layer.h>
#include "../../core.h"
#include "Device.h"

namespace qk::gfx {

	enum class EDebugMessageSeverity {
		VERBOSE_BIT = 0x00000001,
		INFO_BIT = 0x00000010,
		WARNING_BIT = 0x00000100,
		ERROR_BIT = 0x00001000,
		ALL_SEVERITIES = VERBOSE_BIT | INFO_BIT | WARNING_BIT | ERROR_BIT,
	};

	enum class EDebugMessageType {
		GENERAL_BIT = 0x00000001,
		VALIDATION_BIT = 0x00000002,
		PERFORMANCE_BIT = 0x00000004,
		DEVICE_ADDRESS_BINDING_BIT = 0x00000008,
		ALL_TYPES = GENERAL_BIT | VALIDATION_BIT | PERFORMANCE_BIT | DEVICE_ADDRESS_BINDING_BIT
	};

	class QK_API DebugMessenger {

		VkDebugUtilsMessengerEXT m_Messenger = nullptr;

	public:
		DebugMessenger() = default;
		DebugMessenger(VkDebugUtilsMessengerEXT messenger);
		void destroy(const Device& paired_device);

	};


	QK_API DebugMessenger AttachDebugMessenger(const Device& target,
		EDebugMessageSeverity severity,
		EDebugMessageType type,
		void* custom = nullptr
	);




}