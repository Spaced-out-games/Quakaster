#pragma once
#include "../../core.h"
#include <string>
#include <NVRHI/nvrhi.h>
#include <NVRHI/vulkan.h>



namespace qk::gfx {

	// Unique device identifier
	using Device_uuid_t = std::array<uint8_t, 16>;

	using DeviceScore_t = uint64_t;


	// features for a device
	enum class EDeviceFeatureSet : uint32_t {
		NO_FEATURES			= 0,
		GEOMETRY_SHADER		= 1 << 0,
		TESSELLATION_SHADER = 1 << 1,
		RAY_TRACING			= 1 << 2,
	};

	constexpr inline EDeviceFeatureSet operator|(EDeviceFeatureSet a, EDeviceFeatureSet b) {
		return static_cast<EDeviceFeatureSet>(
			static_cast<uint32_t>(a) | static_cast<uint32_t>(b)
			);
	}


	constexpr inline EDeviceFeatureSet operator&(EDeviceFeatureSet a, EDeviceFeatureSet b) {
		return static_cast<EDeviceFeatureSet>(
			static_cast<uint32_t>(a) & static_cast<uint32_t>(b)
			);
	}

	constexpr inline EDeviceFeatureSet& operator|=(EDeviceFeatureSet& a, EDeviceFeatureSet b) {
		a = a | b;
		return a;
	}

	constexpr inline bool has(EDeviceFeatureSet value, EDeviceFeatureSet flag) {
		return (value & flag) == flag;
	}


	// Device vendor
	enum class EDeviceVendor : uint32_t {
		UNKNOWN_VENDOR = 0,
		NVIDIA  = 0x10DE,
		AMD		= 0x1002,
		INTEL	= 0x8086,
	};


	// whether the device is integrated or discrete graphics
	enum class EDeviceType : uint8_t {
		UNKNOWN_DEVICE,
		INTEGRATED_DEVICE,
		DISCRETE_DEVICE
	};

	// Device information
	struct DeviceInfo
	{
		Device_uuid_t		uuid		= {};		// UUID or some other unique device identifier (PCI?)
		uint64_t			vram		= 0;		// total memory in bytes
		EDeviceVendor		vendor		= EDeviceVendor::UNKNOWN_VENDOR;
		EDeviceFeatureSet	features	= EDeviceFeatureSet::NO_FEATURES;
		EDeviceType			type		= EDeviceType::UNKNOWN_DEVICE;
		#ifdef _DEBUG
		std::string			name		= "";
		#endif
	};
	
	// Function that scores a device.
	using DeviceScoring_fn = DeviceScore_t(*)(const DeviceInfo&);


	DeviceInfo vk_GetDeviceInfo(VkPhysicalDevice target);
	DeviceScore_t vk_ScoreDevice(VkPhysicalDevice target, DeviceScoring_fn score_fn = nullptr);




	class QK_API Device {

		friend class DeviceDescBuilder;

		DeviceInfo				m_Info;
		DeviceScore_t			m_Score = 0;

		// introspection-related
		VkInstance				m_Instance		 = VK_NULL_HANDLE;
		VkPhysicalDevice		m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice				m_Device		 = VK_NULL_HANDLE;

		nvrhi::DeviceHandle m_Handle = nullptr;


	public:

		// outstanding idea: override the -> operator so it maintains the ergonomics of a DeviceHandle
		nvrhi::IDevice* operator->() {
			assert(m_Handle != nullptr);
			return m_Handle.Get();
		}

		const nvrhi::IDevice* operator->() const {
			assert(m_Handle != nullptr);
			return m_Handle.Get();
		}

		bool valid() const {
			return m_Handle != nullptr;
		}


		[[nodiscard]] nvrhi::DeviceHandle handle();
		[[nodiscard]] const DeviceInfo& info() const;
		[[nodiscard]] DeviceScore_t score() const;

		VkInstance instance() const;

		#ifdef _DEBUG

			[[nodiscard]] const std::string& name() const;
				
			void rename(const std::string& new_name);

		#endif

	};





}