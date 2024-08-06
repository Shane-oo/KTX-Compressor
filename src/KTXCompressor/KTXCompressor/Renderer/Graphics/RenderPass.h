//
// Created by shane on 28/07/2024.
//

#ifndef KTXCOMPRESSOR_RENDERPASS_H
#define KTXCOMPRESSOR_RENDERPASS_H

#include <vulkan/vulkan_core.h>
#include "../../Common.h"

namespace KTXCompressor {

    class RenderPass {
    public:
        RenderPass(VkDevice vulkanDevice, VkFormat swapChainImageFormat);

        ~RenderPass();

    private:
        VkDevice vulkanDevice;
        VkRenderPass vulkanRenderPass;

        VkRenderPass CreateVulkanRenderPass(VkFormat swapChainImageFormat);

    public:
        void Begin(VkCommandBuffer vulkanCommandBuffer, VkFramebuffer vulkanFrameBuffer, VkExtent2D extent);
        
        void End(VkCommandBuffer vulkanCommandBuffer);
        
        VkRenderPass GetVulkanRenderPass() {
            return vulkanRenderPass;
        }
        
        uint32_t GetSubpassIndex(){
            return 0;
        }
        
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_RENDERPASS_H
