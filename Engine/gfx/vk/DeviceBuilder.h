#pragma once
#include "../../core.h"
#include <string>
#include <nvrhi/nvrhi.h>
#include <vulkan/vulkan.h>
#include "Device.h"
//#include "DebugMessenger.h" //todo: remove this
#ifdef _DEBUG
	#define QK_GFX_USE_VALIDATION_LAYERS
#endif

namespace qk::gfx {


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
		std::string m_ApplicationName;
		uint32_t	m_ApplicationVersion;
		uint32_t	m_APIVersion = 0;
		std::vector<const char*> m_Extensions;
		std::vector<const char*> m_Layers;
		uint32_t				 m_Flags = 0;


		// Physical device creation

		DeviceScoring_fn m_DeviceScoring_fn = nullptr;

		public:

		DeviceDescBuilder();
		~DeviceDescBuilder() = default;


		DeviceDescBuilder& appinfo(const std::string& app_name, uint32_t app_version);

		DeviceDescBuilder& version(uint32_t api_version);

		DeviceDescBuilder& add_extension(const char* extension);

		DeviceDescBuilder& set_device_scoring_fn(DeviceScoring_fn score_fn);

		Device build();



	};
}