#include "Device.h"
#include <cstring>
#include <vector>
#include <spdlog/spdlog.h>

namespace qk::gfx {
	nvrhi::DeviceHandle Device::handle() { return m_Handle; }
	const DeviceInfo&	Device::info() const { return m_Info; }
    DeviceScore_t		Device::score() const { return m_Score; }
    VkInstance Device::instance() const { return m_Instance; }


    DeviceInfo vk_GetDeviceInfo(VkPhysicalDevice target)
    {
        DeviceInfo info{};

        if (target == VK_NULL_HANDLE) {
            return info;
            spdlog::warn("qk::gfx::vk_GetDeviceInfo: `target` was null.");
        }


        VkPhysicalDeviceIDProperties idProps{};
        idProps.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES;

        VkPhysicalDeviceProperties2 props2{};
        props2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
        props2.pNext = &idProps;

        vkGetPhysicalDeviceProperties2(target, &props2);

        const VkPhysicalDeviceProperties& props = props2.properties;

        #ifdef _DEBUG
        info.name = props.deviceName;
        #endif
        



        for (size_t i = 0; i < 16; i++)
            info.uuid[i] = idProps.deviceUUID[i];

        


        switch (props.vendorID)
        {
        case 0x10DE: info.vendor = EDeviceVendor::NVIDIA; break;
        case 0x1002: info.vendor = EDeviceVendor::AMD;    break;
        case 0x8086: info.vendor = EDeviceVendor::INTEL;  break;
        default:     info.vendor = EDeviceVendor::UNKNOWN_VENDOR; break;
        }

        


        switch (props.deviceType)
        {
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            info.type = EDeviceType::DISCRETE_DEVICE;
            break;

        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            info.type = EDeviceType::INTEGRATED_DEVICE;
            break;

        default:
            info.type = EDeviceType::UNKNOWN_DEVICE;
            break;
        }

        


        VkPhysicalDeviceMemoryProperties memProps{};
        vkGetPhysicalDeviceMemoryProperties(target, &memProps);

        uint64_t totalVRAM = 0;

        for (uint32_t i = 0; i < memProps.memoryHeapCount; i++)
        {
            const VkMemoryHeap& heap = memProps.memoryHeaps[i];

            if (heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
            {
                totalVRAM += heap.size;
            }
        }

        info.vram = totalVRAM;

        


        VkPhysicalDeviceFeatures features{};
        vkGetPhysicalDeviceFeatures(target, &features);

        if (features.geometryShader)
            info.features |= EDeviceFeatureSet::GEOMETRY_SHADER;

        if (features.tessellationShader)
            info.features |= EDeviceFeatureSet::TESSELLATION_SHADER;

        


        uint32_t extCount = 0;
        vkEnumerateDeviceExtensionProperties(target, nullptr, &extCount, nullptr);

        std::vector<VkExtensionProperties> extensions(extCount);
        vkEnumerateDeviceExtensionProperties(target, nullptr, &extCount, extensions.data());

        bool hasRT = false;

        for (const auto& ext : extensions)
        {
            if (strcmp(ext.extensionName, VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME) == 0 ||
                strcmp(ext.extensionName, VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME) == 0)
            {
                hasRT = true;
                break;
            }
        }

        if (hasRT)
            info.features |= EDeviceFeatureSet::RAY_TRACING;

        return info;
    }

    DeviceScore_t vk_DefaultDeviceScore(const DeviceInfo& info)
    {

        using enum EDeviceType;
        using enum EDeviceVendor;
        using enum EDeviceFeatureSet;


        if (info.type == UNKNOWN_DEVICE && info.vendor == UNKNOWN_VENDOR && info.vram == 0) {
            return 0;
        }

        DeviceScore_t score = 0;

        // ------------------------------------------------------------
        // 1. Device type (most important signal)
        // ------------------------------------------------------------
        switch (info.type)
            {
            case DISCRETE_DEVICE:
                score += 1'000'000;
                break;

            case INTEGRATED_DEVICE:
                score += 100'000;
                break;

            default:
                score += 10'000; // fallback / virtual / unknown
                break;
        }

        // ------------------------------------------------------------
        // 2. VRAM (scale in MB to avoid huge numbers)
        // ------------------------------------------------------------
        // clamp-ish behavior: avoids absurd dominance from weird heaps
        const DeviceScore_t vram_mb = info.vram / (1024 * 1024);
        score += vram_mb;

        // ------------------------------------------------------------
        // 3. Feature support (capability bonus)
        // ------------------------------------------------------------
        if (has(info.features, GEOMETRY_SHADER))
            score += 10'000;

        if (has(info.features, TESSELLATION_SHADER))
            score += 10'000;

        if (has(info.features, RAY_TRACING))
            score += 200'000; // strong but not dominant

        // ------------------------------------------------------------
        // 4. Vendor bias (very mild, just stability preference)
        // ------------------------------------------------------------
        switch (info.vendor)
        {
        case NVIDIA:
            score += 5'000;
            break;

        case AMD:
            score += 4'000;
            break;

        case INTEL:
            score += 2'000;
            break;

        default:
            break;
        }

        

        return score;
    }


    DeviceScore_t vk_ScoreDevice(VkPhysicalDevice target, DeviceScoring_fn score_fn) {

        // get the device info
        DeviceInfo device_info = vk_GetDeviceInfo(target);

        // try to score it

        if (score_fn) {
            return score_fn(device_info);
        }

        return vk_DefaultDeviceScore(device_info);

    }


    #ifdef _DEBUG

    [[nodiscard]] const std::string& Device::name() const {
        return m_Info.name;
    }

    void Device::rename(const std::string& new_name) {
        m_Info.name = new_name;
    }

    #endif

}