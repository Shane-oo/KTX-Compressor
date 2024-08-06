//
// Created by shane on 6/08/2024.
//

#ifndef KTXCOMPRESSOR_DRAWCOMMAND_H
#define KTXCOMPRESSOR_DRAWCOMMAND_H

#include <vulkan/vulkan_core.h>
#include "../../Common.h"

namespace KTXCompressor {

    class GraphicsPipeline;


    class DrawCommand {
    public:
        DrawCommand(VkDevice vulkanDevice, uint32_t graphicsFamilyIndex);

        ~DrawCommand();

    private:
        VkDevice vulkanDevice;
        VkCommandPool vulkanCommandPool;
        vector<VkCommandBuffer> vulkanCommandBuffers;


        VkCommandPool CreateVulkanCommandPool(uint32_t graphicsFamilyIndex);

        vector<VkCommandBuffer> CreateVulkanCommandBuffers();

    public:
        void Begin(uint32_t currentFrame);

        void End(uint32_t currentFrame);

        VkCommandBuffer GetVulkanCommandBuffer(uint32_t currentFrame) {
            return vulkanCommandBuffers[currentFrame];
        }
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_DRAWCOMMAND_H
