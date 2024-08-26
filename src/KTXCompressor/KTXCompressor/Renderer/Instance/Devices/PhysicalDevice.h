//
// Created by shane on 7/07/2024.
//

#ifndef KTXCOMPRESSOR_PHYSICALDEVICE_H
#define KTXCOMPRESSOR_PHYSICALDEVICE_H

#include "../../../Common.h"
#include "Queues/QueueFamily.h"
#include <vulkan/vulkan_core.h>

namespace KTXCompressor {

    class PhysicalDevice {

    public:
        PhysicalDevice(VkInstance vulkanInstance, QueueFamily *queueFamily);

        ~PhysicalDevice();

        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags memoryPropertyFlags);

    private:
        VkPhysicalDevice vulkanPhysicalDevice;
        VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
        VkPhysicalDeviceProperties physicalDeviceProperties;
        QueueFamily *queueFamily;

    private:
        VkPhysicalDevice PickPhysicalDevice(VkInstance vulkanInstance);

        bool IsDeviceSuitable(VkPhysicalDevice device);

        bool CheckDeviceExtensionsSupported(VkPhysicalDevice device);

    public:

        uint32_t GetGraphicsFamilyIndex() {
            return queueFamily->GetSelectedQueueFamilyIndices().graphicsFamilyIndex.value();
        }

        uint32_t GetPresentFamilyIndex() {
            return queueFamily->GetSelectedQueueFamilyIndices().presentFamilyIndex.value();
        }

        [[nodiscard]] VkPhysicalDevice GetVulkanPhysicalDevice() const {
            return vulkanPhysicalDevice;
        }

        [[nodiscard]] VkPhysicalDeviceFeatures GetRequiredPhysicalDeviceFeatures() {
            VkPhysicalDeviceFeatures requiredPhysicalDeviceFeatures{};
            requiredPhysicalDeviceFeatures.samplerAnisotropy = VK_TRUE;
            return requiredPhysicalDeviceFeatures;
        }

        [[nodiscard]] float GetMaxSamplerAnisotropy() const {
            return physicalDeviceProperties.limits.maxSamplerAnisotropy;
        }
    };


} // KTXCompressor

#endif //KTXCOMPRESSOR_PHYSICALDEVICE_H
