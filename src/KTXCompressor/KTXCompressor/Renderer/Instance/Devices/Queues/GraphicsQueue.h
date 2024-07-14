//
// Created by shane on 14/07/2024.
//

#ifndef KTXCOMPRESSOR_GRAPHICSQUEUE_H
#define KTXCOMPRESSOR_GRAPHICSQUEUE_H

#include <vulkan/vulkan_core.h>

namespace KTXCompressor {

    class GraphicsQueue {

    public:
        GraphicsQueue(VkQueue vkQueue);
        
    private:
        VkQueue vulkanQueue;
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_GRAPHICSQUEUE_H
