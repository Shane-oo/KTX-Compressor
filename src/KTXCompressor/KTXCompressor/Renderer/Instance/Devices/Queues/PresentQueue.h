//
// Created by shane on 14/07/2024.
//

#ifndef KTXCOMPRESSOR_PRESENTQUEUE_H
#define KTXCOMPRESSOR_PRESENTQUEUE_H

#include <vulkan/vulkan_core.h>

namespace KTXCompressor {

    class PresentQueue {

    public:
        PresentQueue(VkQueue vkQueue);

    private:
        VkQueue vulkanQueue;
    };


} // KTXCompressor

#endif //KTXCOMPRESSOR_PRESENTQUEUE_H
