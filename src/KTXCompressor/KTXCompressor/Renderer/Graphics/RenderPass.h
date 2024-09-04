//
// Created by shane on 28/07/2024.
//

#ifndef KTXCOMPRESSOR_RENDERPASS_H
#define KTXCOMPRESSOR_RENDERPASS_H

#include <vulkan/vulkan_core.h>
#include "../../Common.h"
#include "../Instance/Devices/PhysicalDevice.h"
#include "../Instance/Devices/LogicalDevice.h"

namespace KTXCompressor {

    class RenderPass {
    public:
        RenderPass(PhysicalDevice* physicalDevice,LogicalDevice* logicalDevice, VkFormat swapChainImageFormat);

        ~RenderPass();

    private:
        PhysicalDevice* physicalDevice;
        LogicalDevice* logicalDevice;
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
        
        bool NeedsDepthAttachment(){
            // im gui would be false
            return true;
        }
        
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_RENDERPASS_H
