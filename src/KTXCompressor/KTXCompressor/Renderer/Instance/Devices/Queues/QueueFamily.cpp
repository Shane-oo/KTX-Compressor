//
// Created by shane on 14/07/2024.
//

#include "QueueFamily.h"

namespace KTXCompressor {
    // #region Private Static Methods

    QueueFamilyIndices QueueFamilyIndices::FindQueueFamilies(VkPhysicalDevice vulkanPhysicalDevice) {
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
            if (indices.IsComplete()) {
                // Found what we are looking for
                break;
            }

            i++;
        }


        return indices;
    }
    // #endregion


    // #region Private Methods


    // #endregion


} // KTXCompressor