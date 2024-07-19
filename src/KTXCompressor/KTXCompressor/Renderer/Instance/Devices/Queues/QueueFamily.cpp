//
// Created by shane on 14/07/2024.
//

#include "QueueFamily.h"

namespace KTXCompressor {


    // #region Constructor

    QueueFamily::QueueFamily(VkSurfaceKHR vulkanSurface) {
        this->vulkanSurface = vulkanSurface;
    }

    // #endregion

    // #region Public Methods
    QueueFamily::QueueFamilyIndices
    QueueFamily::FindQueueFamiliesForPhysicalDevice(VkPhysicalDevice vulkanPhysicalDevice) {

        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(vulkanPhysicalDevice, &queueFamilyCount, nullptr);

        vector<VkQueueFamilyProperties> queueFamiliesProperties(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(vulkanPhysicalDevice, &queueFamilyCount,
                                                 queueFamiliesProperties.data());

        int i = 0;
        for (const auto &queueFamilyProperties: queueFamiliesProperties) {
            if (queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamilyIndex = i;
            }

            // Look for a queue family that has capability of presenting to our window surface
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(vulkanPhysicalDevice, i, vulkanSurface, &presentSupport);

            if (presentSupport) {
                indices.presentFamilyIndex = i;
            }


            if (indices.IsComplete()) {
                // Found all we are looking for
                break;
            }

            i++;
        }

        return indices;
    }
// #endregion

} // KTXCompressor