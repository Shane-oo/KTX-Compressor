//
// Created by shane on 28/07/2024.
//

#ifndef KTXCOMPRESSOR_RENDERPASS_H
#define KTXCOMPRESSOR_RENDERPASS_H

#include <vulkan/vulkan_core.h>
#include "../../../Common.h"
#include "../../Instance/Devices/PhysicalDevice.h"
#include "../../Instance/Devices/LogicalDevice.h"

namespace KTXCompressor {

    class RenderPass {
    public:
        RenderPass(PhysicalDevice *physicalDevice,
                   LogicalDevice *logicalDevice,
                   VkFormat swapChainImageFormat,
                   bool isFirstRenderPass,
                   bool isLastRenderPass);

        ~RenderPass();

    protected:
        void Init();

        virtual VkRenderPass CreateVulkanRenderPass() = 0;

        virtual void SetClearValues(VkRenderPassBeginInfo &renderPassBeginInfo);


        PhysicalDevice *physicalDevice;
        LogicalDevice *logicalDevice;
        VkFormat swapChainImageFormat;
        bool isFirstRenderPass;
        bool isLastRenderPass;
    private:
        VkRenderPass vulkanRenderPass;

        array<VkClearValue, 2> defaultClearValues{};

    public:
        void Begin(VkCommandBuffer vulkanCommandBuffer, VkFramebuffer vulkanFrameBuffer, VkExtent2D extent);

        void End(VkCommandBuffer vulkanCommandBuffer);

        VkRenderPass GetVulkanRenderPass() {
            return vulkanRenderPass;
        }

        uint32_t GetSubpassIndex() {
            return 0;
        }

        virtual bool NeedsDepthAttachment() {
            return true;
        }

        virtual void Render(VkCommandBuffer vulkanCommandBuffer);

    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_RENDERPASS_H
