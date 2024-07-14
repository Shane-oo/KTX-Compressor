//
// Created by shane on 14/07/2024.
//

#ifndef KTXCOMPRESSOR_QUEUEFAMILY_H
#define KTXCOMPRESSOR_QUEUEFAMILY_H

#include "../../../../Common.h"
#include <vulkan/vulkan_core.h>

namespace KTXCompressor {
    struct QueueFamilyIndices {
        optional<uint32_t> graphicsFamilyIndex;

        static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice vulkanPhysicalDevice);

        bool IsComplete() {
            return graphicsFamilyIndex.has_value();
        }
    };


    class QueueFamily {


    private:


    private:

    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_QUEUEFAMILY_H
