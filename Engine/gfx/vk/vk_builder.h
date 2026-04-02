#pragma once
#include "../../core.h"
#include <string>
#include <nvrhi/nvrhi.h>
//#include <nvrhi/vulkan.h>

#ifdef _DEBUG
	#define QK_GFX_USE_VALIDATION_LAYERS
#endif

typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkInstance_T* VkInstance;
typedef struct VkDevice_T* VkDevice;

namespace qk::gfx::vk {



	enum class EDeviceFeatureSet: uint32_t {
		GEOMETRY_SHADER = 1 << 0,
		TESSELLATION_SHADER = 1 << 1,
		RAY_TRACING = 1 << 2,
	};

	enum class EDeviceType: uint8_t {
		INTEGRATED_DEVICE,
		DISCRETE_DEVICE
	};

	// plan: since nvrhi can handle validation layers for us, we can *probably* cut that out and let nvhri handle it,which lets us remove BuildInfo!!!
	// Armed with a heuristic, we just select [0] for the PhysicalDevice
	// merge queue family requirements into FeatureSet
	// 
	// Need to check up LogicalDevice and VkQueue patterns first...
	// maybe combine the device handle and the validation layer in a whole Device

	struct DeviceInfo
	{
		EDeviceFeatureSet	device_features;
		EDeviceType			device_type;
		uint32_t			device_vendor;
		uint32_t			device_id;
		uint64_t			device_vram; // total memory in bytes
	};

	// defined in implementation
	struct BuildInfo;

	struct QK_API Device;


	/* Helper that generates:
		VkInstance, VkPhysicalDevice, VkDevice, VkQueue graphics queue, VkQueue transfer queue (optional)
		then returns it as a nvhri::DeviceHandle

		TODO: consider hooking debugCallback up to Quakaster's event system?
	*/
	struct QK_API Initializer {
		public:
			using DeviceHeuristic = size_t(*)(const DeviceInfo&);
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

		Initializer();
		~Initializer();

		Initializer& set_device_heuristic(DeviceHeuristic heuristic);

		Initializer& set_application_details(const std::string& app_name, uint32_t app_version);

		Initializer& set_API_version(uint32_t api_version);

		Initializer& add_extension(const char* extension);

		Initializer& add_layer(const char* layer);

		// Uses the heuristic function to map indices to the sorted end-product. For example, if device 2 is 'best' vec[0] = 2. This keeps the end-user from seeing Vulkan internals.
		const std::vector <uint32_t> device_map();

		Initializer& build_instance();
		
		Initializer& use_device(uint32_t device_id);



		Device build_device() const;
	};
	// for now; move to 
	struct QK_API Device {};
}