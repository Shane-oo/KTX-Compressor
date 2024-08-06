//
// Created by shane on 14/07/2024.
//

#ifndef KTXCOMPRESSOR_QUEUE_H
#define KTXCOMPRESSOR_QUEUE_H

#include <vulkan/vulkan_core.h>

namespace KTXCompressor {

    class Queue {

    public:
        Queue(VkQueue vkQueue);

    private:
        VkQueue vulkanQueue;

    public:
        VkQueue GetVulkanQueue() {
            return vulkanQueue;
        }
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_QUEUE_H
