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
        PhysicalDevice(VkInstance vulkanInstance);

        ~PhysicalDevice();

    private:
        VkPhysicalDevice vulkanPhysicalDevice;
        QueueFamilyIndices queueFamilyIndices;

    private:
        VkPhysicalDevice PickPhysicalDevice(VkInstance vulkanInstance);

        bool IsDeviceSuitable(VkPhysicalDevice device);

    public:
        QueueFamilyIndices GetQueueFamilyIndices() {
            return queueFamilyIndices;
        }

        [[nodiscard]] VkPhysicalDevice GetVulkanPhysicalDevice() const {
            return vulkanPhysicalDevice;
        }

        [[nodiscard]] VkPhysicalDeviceFeatures GetRequiredPhysicalDeviceFeatures() {
            // todo
            VkPhysicalDeviceFeatures enabledDeviceFeatures{};
            return enabledDeviceFeatures;
        }
    };


} // KTXCompressor

#endif //KTXCOMPRESSOR_PHYSICALDEVICE_H
