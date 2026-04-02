#pragma once
#include "../../core.h"
#include <string>
#include <nvrhi/nvrhi.h>
//#include <nvrhi/vulkan.h>

#ifdef _DEBUG
	#define QK_GFX_USE_VALIDATION_LAYERS
#endif
typedef struct VkInstance_T* VkInstance;

namespace qk::gfx::vk {


	enum PhysicalDeviceFeatureSet {
		GEOMETRY_SHADER = 1,

	};


	struct PhysicalDeviceInfo
	{
		uint32_t vendorID;
		uint32_t deviceID;
		uint32_t type; // e.g., discrete, integrated, virtual
		uint64_t vram; // total memory in bytes
	};

	// defined in implementation
	struct BuildInfo;


	/* Helper that generates:
		VkInstance, VkPhysicalDevice, VkDevice, VkQueue graphics queue, VkQueue transfer queue (optional)
		then returns it as a nvhri::DeviceHandle

		TODO: consider hooking debugCallback up to Quakaster's event system?
	*/
	struct QK_API Builder {
		public:
			using DeviceHeuristic = bool(*)(const PhysicalDeviceInfo&, const PhysicalDeviceInfo&);
		private:

		BuildInfo* m_Info = nullptr;

		// VkApplicationInfo
		std::string m_ApplicationName;
		uint32_t	m_ApplicationVersion;
		uint32_t	m_APIVersion = 0;



		// vkInstanceCreateInfo
		std::vector<const char*> m_Extensions;
		std::vector<const char*> m_Layers;
		uint32_t				 m_Flags = 0;

		// VkPhysicalDevice
		DeviceHeuristic m_DeviceHeuristic = nullptr;
		uint32_t		m_DeviceID = 0;


		bool check_extension_support() const;
		bool check_layer_support() const;
		void add_debug_manager(VkInstance) const;

		
		public:

		Builder();

		Builder& set_device_heuristic(DeviceHeuristic heuristic);

		Builder& set_application_details(const std::string& app_name, uint32_t app_version);

		Builder& set_API_version(uint32_t api_version);

		Builder& add_extension(const char* extension);

		Builder& add_layer(const char* layer);

		// don't forget to check for nullptr on the heuristic
		Builder& devices(std::vector<uint32_t>& device_map);

		Builder& build_instance();
		
		Builder& use_device(uint32_t device_id);



		nvrhi::DeviceHandle build_device() const;
	};
}