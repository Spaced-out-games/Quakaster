#pragma once
#include "../../core.h"
#include <string>
#include <nvrhi/nvrhi.h>
//#include <vulkan/vulkan.h>
#include "Device.h"
#include <span>
//#include "DebugMessenger.h" //todo: remove this
#ifdef _DEBUG
	#define QK_GFX_USE_VALIDATION_LAYERS
#endif

namespace qk::gfx {

	void QK_API get_glfw_instance_extensions(std::vector<const char*>& out);

	bool QK_API vk_instance_add(
		nvrhi::vulkan::DeviceDesc& desc,
		const std::string& app_name,
		uint32_t app_version,
		uint32_t api_version,
		std::span<const char*> extensions,
		std::span<const char*> layers,
		uint32_t flags
	);

	bool QK_API vk_instance_supports_extensions(std::span<const char*> queried_extensions);

	bool QK_API vk_physical_device_add(nvrhi::vulkan::DeviceDesc& desc, DeviceScoring_fn func);

	void QK_API vk_add_queue_families(nvrhi::vulkan::DeviceDesc& desc, EQueueFeatureSet features);

	bool QK_API vk_device_add(nvrhi::vulkan::DeviceDesc& desc, float queuePriority, std::span<const char*> device_extensions);

	// ------------------------------------------------- Instance Creation ------------------------------------------------- //

	// Final design: DeviceDescBuilder -> DeviceDescBuilder
	// Debug layers are queried explicitly, outside the nvrhi exosystem
	// We just simplify the bureaucracy. We'll make a DeviceTelemetry type, 
	// then a DeviceTelemetry type (unordered_map<(const void*)DeviceHandle, DeviceTelemetry>)
	// in entt::context. From there, anything with an entt::registry/scene can query it.

	struct QK_API DeviceDescBuilder {
		public:
		private:

		// Instance creation
		std::string m_ApplicationName = "Quakaster Client";
		uint32_t	m_ApplicationVersion = QK_MAKE_VERSION(0,0,0);

		// Vulkan v. 1.1
		uint32_t	m_APIVersion		 = QK_MAKE_API_VERSION(0, 1, 1, 0);
		std::vector<const char*> m_Extensions;
		std::vector<const char*> m_DeviceExtensions;
		std::vector<const char*> m_Layers;
		uint32_t				 m_Flags = 0;


		// Physical device creation
		DeviceScoring_fn m_DeviceScoring_fn = nullptr;

		// Queue creation
		EDeviceFeatureSet m_Features = EDeviceFeatureSet::NO_FEATURES;
		EQueueFeatureSet  m_QueueFeatures = EQueueFeatureSet::GRAPHICS | EQueueFeatureSet::TRANSFER;
		public:

		DeviceDescBuilder();
		~DeviceDescBuilder() = default;

		DeviceDescBuilder& appinfo(const std::string& app_name, uint32_t app_version);

		DeviceDescBuilder& version(uint32_t api_version);

		DeviceDescBuilder& extension(const char* ext);

		DeviceDescBuilder& extensions(std::span<const char* const> exts);

		DeviceDescBuilder& device_extension(const char* ext);

		DeviceDescBuilder& device_extensions(std::span<const char* const> exts);

		DeviceDescBuilder& device_features(EDeviceFeatureSet device_features);

		DeviceDescBuilder& queue_features(EQueueFeatureSet queue_features);

		DeviceDescBuilder& enable_validation(bool enable);

		template<typename... Exts>
		DeviceDescBuilder& extensions(Exts... exts)
		{
			(m_Extensions.push_back(exts), ...);
			return *this;
		}

		template<typename... Exts>
		DeviceDescBuilder& device_extensions(Exts... exts)
		{
			(m_DeviceExtensions.push_back(exts), ...);
			return *this;
		}

		DeviceDescBuilder& set_device_scoring_fn(DeviceScoring_fn score_fn);

		nvrhi::vulkan::DeviceDesc build();

		#ifdef QK_USE_GLFW
			DeviceDescBuilder& add_glfw_extensions();
		#endif

	};
}