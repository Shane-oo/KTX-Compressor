//
// Created by shane on 6/08/2024.
//

#ifndef KTXCOMPRESSOR_DRAWCOMMAND_H
#define KTXCOMPRESSOR_DRAWCOMMAND_H

#include <vulkan/vulkan_core.h>
#include "../../Common.h"
#include "Command.h"

namespace KTXCompressor {

    class GraphicsPipeline;


    class DrawCommand : public Command {
    public:
        DrawCommand(LogicalDevice* logicalDevice);

        ~DrawCommand();

    private:

        vector<VkCommandBuffer> vulkanDrawCommandBuffers;

        vector<VkCommandBuffer> CreateVulkanCommandBuffers();

    public:
        void Begin(uint32_t currentFrame);

        void End(uint32_t currentFrame);

        VkCommandBuffer GetVulkanCommandBuffer(uint32_t currentFrame) {
            return vulkanDrawCommandBuffers[currentFrame];
        }
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_DRAWCOMMAND_H
