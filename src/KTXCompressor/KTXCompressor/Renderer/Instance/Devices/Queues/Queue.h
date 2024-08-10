//
// Created by shane on 14/07/2024.
//

#ifndef KTXCOMPRESSOR_QUEUE_H
#define KTXCOMPRESSOR_QUEUE_H

#include <vulkan/vulkan_core.h>

namespace KTXCompressor {

    class Queue {

    public:
        Queue(VkQueue vkQueue, uint32_t queueFamilyIndex);

    private:
        VkQueue vulkanQueue;
        uint32_t queueFamilyIndex;

    public:
        VkQueue GetVulkanQueue() {
            return vulkanQueue;
        }

        uint32_t GetQueueFamilyIndex() {
            return queueFamilyIndex;
        };
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_QUEUE_H
