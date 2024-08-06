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
        VkCommandBuffer vulkanCommandBuffer;


        VkCommandPool CreateVulkanCommandPool(uint32_t graphicsFamilyIndex);

        VkCommandBuffer CreateVulkanCommandBuffer();

    public:
        void Begin();
        
        void End();

        VkCommandBuffer GetVulkanCommandBuffer() {
            return vulkanCommandBuffer;
        }
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_DRAWCOMMAND_H
