#include "DeviceBuilder.h"
#include <nvrhi/vulkan.h>
#ifdef QK_USE_GLFW
#include <GLFW/glfw3.h>
#endif
#include <spdlog/spdlog.h>
//#include <optional>
#include <set>

namespace qk::gfx {

	

	void get_glfw_instance_extensions(std::vector<const char*>& out)
	{
		#ifdef QK_USE_GLFW
		uint32_t count = 0;
		const char** exts = glfwGetRequiredInstanceExtensions(&count);

		if (!exts)
			throw std::runtime_error("GLFW: Vulkan not supported");

		out.insert(out.end(), exts, exts + count);
		#else
		(void)out; // no-op on non-GLFW builds
		#endif
	}








	bool vk_instance_add(
		nvrhi::vulkan::DeviceDesc& desc,
		const std::string& app_name,
		uint32_t app_version,
		uint32_t api_version,
		std::span<const char*> extensions,
		std::span<const char*> layers,
		uint32_t flags
	) {

		desc.instanceExtensions = extensions.data();
		desc.numInstanceExtensions = (uint32_t)extensions.size();

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
		VkResult result = vkCreateInstance(&createinfo, nullptr, &desc.instance);

		if (result != VK_SUCCESS) {
			spdlog::error("vkCreateInstance() failure!");
			return false;
		}
		else {
			spdlog::info("vkCreateInstance() success!");
		}


		return true;

	}



	bool vk_physical_device_add(nvrhi::vulkan::DeviceDesc& desc, DeviceScoring_fn func) {

		assert(desc.instance != VK_NULL_HANDLE);

		// Get the devices
		uint32_t num_devices = 0;
		vkEnumeratePhysicalDevices(desc.instance, &num_devices, nullptr);
		std::vector<VkPhysicalDevice> devices(num_devices);
		vkEnumeratePhysicalDevices(desc.instance, &num_devices, devices.data());

		// get the scores
		DeviceScore_t high_score = 0;
		DeviceScore_t curr_score = 0;
		int			  best_device_index = -1;

		for (int i = 0; i < num_devices; i++) {
			curr_score = vk_ScoreDevice(devices[i], func);
			if (curr_score > high_score) {
				high_score = curr_score;
				best_device_index = i;
			}
		}

		if (best_device_index == -1) desc.physicalDevice = VK_NULL_HANDLE;

		if (best_device_index == -1)
		{
			spdlog::error("No suitable physical device found.");
			desc.physicalDevice = VK_NULL_HANDLE;
			return false;
		}

		desc.physicalDevice = devices.at(best_device_index);
		assert(desc.physicalDevice != VK_NULL_HANDLE && "No physical device selected");
		return true;
	}


	


	

	// checks for extension support
	bool vk_instance_supports_extensions(std::span<const char*> queried_extensions) {

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

	void vk_add_queue_families(nvrhi::vulkan::DeviceDesc& desc, EQueueFeatureSet features) {
		assert(desc.physicalDevice != VK_NULL_HANDLE);
		uint32_t num_queue_families = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(desc.physicalDevice, &num_queue_families, nullptr);

		std::vector<VkQueueFamilyProperties> queue_families(num_queue_families);
		vkGetPhysicalDeviceQueueFamilyProperties(desc.physicalDevice, &num_queue_families, queue_families.data());

		using enum EQueueFeatureSet;

		desc.graphicsQueueIndex = -1;
		desc.transferQueueIndex = -1;
		desc.computeQueueIndex = -1;

		for (uint32_t i = 0; i < num_queue_families; i++)
		{
			const auto& queueFamily = queue_families[i];

			if ((desc.graphicsQueueIndex == -1) &&
				has(features, GRAPHICS) &&
				(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT))
			{
				desc.graphicsQueueIndex = static_cast<int>(i);
			}

			if ((desc.transferQueueIndex == -1) &&
				has(features, TRANSFER) &&
				(queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT))
			{
				desc.transferQueueIndex = static_cast<int>(i);
			}

			if ((desc.computeQueueIndex == -1) &&
				has(features, COMPUTE) &&
				(queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT))
			{
				desc.computeQueueIndex = static_cast<int>(i);
			}
		}

		// just some assertions

		#ifdef _DEBUG
		if (has(features, GRAPHICS)) assert(desc.graphicsQueueIndex != -1);

		if (has(features, TRANSFER)) assert(desc.transferQueueIndex != -1);

		if (has(features, COMPUTE)) assert(desc.computeQueueIndex != -1);

		

		#endif

	}

	bool vk_device_add(nvrhi::vulkan::DeviceDesc& desc, float queuePriority, std::span<const char*> device_extensions)
	{
		assert(desc.physicalDevice != VK_NULL_HANDLE);

		assert(
			(desc.graphicsQueueIndex != -1 ||
			desc.transferQueueIndex != -1 ||
			desc.computeQueueIndex != -1)
			&& "Attempting to make a device without any features"
		);

		desc.deviceExtensions = device_extensions.data();
		desc.numDeviceExtensions = device_extensions.size();


		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies;

		if (desc.graphicsQueueIndex != -1)
			uniqueQueueFamilies.insert(desc.graphicsQueueIndex);

		if (desc.transferQueueIndex != -1)
			uniqueQueueFamilies.insert(desc.transferQueueIndex);

		if (desc.computeQueueIndex != -1)
			uniqueQueueFamilies.insert(desc.computeQueueIndex);

		for (uint32_t family : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueInfo{};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = family;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &queuePriority;

			queueCreateInfos.push_back(queueInfo);
		}

		VkPhysicalDeviceFeatures features{}; // expand later

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount =
			static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.enabledExtensionCount =
			static_cast<uint32_t>(desc.numDeviceExtensions);
		createInfo.ppEnabledExtensionNames = desc.deviceExtensions;

		createInfo.pEnabledFeatures = &features;

		VkResult result = vkCreateDevice(
			desc.physicalDevice,
			&createInfo,
			desc.allocationCallbacks,
			&desc.device
		);

		if (result != VK_SUCCESS)
		{
			spdlog::error("vkCreateDevice() failed!");
			return false;
		}

		if (desc.graphicsQueueIndex != -1)
		{
			vkGetDeviceQueue(
				desc.device,
				desc.graphicsQueueIndex,
				0,
				&desc.graphicsQueue
			);
		}

		if (desc.transferQueueIndex != -1)
		{
			vkGetDeviceQueue(
				desc.device,
				desc.transferQueueIndex,
				0,
				&desc.transferQueue
			);
		}

		if (desc.computeQueueIndex != -1)
		{
			vkGetDeviceQueue(
				desc.device,
				desc.computeQueueIndex,
				0,
				&desc.computeQueue
			);
		}

		return true;
	}



	// --------------------------------------------  DEVICE CREATION  -------------------------------------------- //

	DeviceDescBuilder::DeviceDescBuilder() {

		device_extensions(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	}

	DeviceDescBuilder& DeviceDescBuilder::enable_validation(bool enable) {

		if(enable) m_Extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		return *this;
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

	DeviceDescBuilder& DeviceDescBuilder::extension(const char* ext)
	{
		m_Extensions.push_back(ext);
		return *this;
	}

	DeviceDescBuilder& DeviceDescBuilder::extensions(std::span<const char* const> exts)
	{
		m_Extensions.insert(m_Extensions.end(), exts.begin(), exts.end());
		return *this;
	}

	DeviceDescBuilder& DeviceDescBuilder::device_extension(const char* ext)
	{
		m_DeviceExtensions.push_back(ext);
		return *this;
	}

	DeviceDescBuilder& DeviceDescBuilder::device_extensions(std::span<const char* const> exts)
	{
		m_DeviceExtensions.insert(m_DeviceExtensions.end(), exts.begin(), exts.end());
		return *this;
	}

	DeviceDescBuilder& DeviceDescBuilder::device_features(EDeviceFeatureSet device_features) {
		m_Features = device_features;
		return *this;
	}
	DeviceDescBuilder& DeviceDescBuilder::queue_features(EQueueFeatureSet queue_features) {
		m_QueueFeatures = queue_features;
		return *this;
	}

	nvrhi::vulkan::DeviceDesc DeviceDescBuilder::build()
	{
		nvrhi::vulkan::DeviceDesc desc{};

		bool status = vk_instance_supports_extensions(m_Extensions);

		status = status = vk_instance_add(
			desc,
			m_ApplicationName,
			m_ApplicationVersion,
			m_APIVersion,
			m_Extensions,
			m_Layers,
			m_Flags
		);

		vk_physical_device_add(desc, m_DeviceScoring_fn);

		vk_add_queue_families(desc, m_QueueFeatures);

		vk_device_add(desc, 1.0f, m_DeviceExtensions);

		return desc;
	}

	#ifdef QK_USE_GLFW
	DeviceDescBuilder& DeviceDescBuilder::add_glfw_extensions() {
		get_glfw_instance_extensions(m_Extensions);
		return *this;
	}
	#endif




}