//
// Created by shane on 14/07/2024.
//

#ifndef KTXCOMPRESSOR_QUEUEFAMILY_H
#define KTXCOMPRESSOR_QUEUEFAMILY_H

#include "../../../../Common.h"
#include <vulkan/vulkan_core.h>

namespace KTXCompressor {

    class QueueFamily {

    public:
        QueueFamily(VkSurfaceKHR vulkanSurface);


    private:
        VkSurfaceKHR vulkanSurface;

        struct QueueFamilyIndices {
            optional<uint32_t> graphicsFamilyIndex;
            optional<uint32_t> presentFamilyIndex;


            [[nodiscard]] bool IsComplete() const {
                return graphicsFamilyIndex.has_value() && presentFamilyIndex.has_value();
            }
        };

        QueueFamilyIndices selectedQueueFamilyIndices;


    public:
        QueueFamilyIndices FindQueueFamiliesForPhysicalDevice(VkPhysicalDevice vulkanPhysicalDevice);

    public:
        QueueFamilyIndices GetSelectedQueueFamilyIndices() const {
            return selectedQueueFamilyIndices;
        }

        void SetSelectedQueueFamilyIndices(QueueFamilyIndices selectedQueueFamilyIndices) {
            this->selectedQueueFamilyIndices = selectedQueueFamilyIndices;
        }
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_QUEUEFAMILY_H
