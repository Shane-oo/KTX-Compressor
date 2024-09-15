//
// Created by shane on 28/07/2024.
//

#include "RenderPass.h"

namespace KTXCompressor {

    // #region Protected Methods

    void RenderPass::Init() {
        vulkanRenderPass = CreateVulkanRenderPass();
    }


    void RenderPass::SetClearValues(VkRenderPassBeginInfo &renderPassBeginInfo) {

        renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(defaultClearValues.size());;
        renderPassBeginInfo.pClearValues = defaultClearValues.data();
    }



    // #endregion

    // #region Constructors

    RenderPass::RenderPass(PhysicalDevice *physicalDevice,
                           LogicalDevice *logicalDevice,
                           VkFormat swapChainImageFormat,
                           bool isFirstRenderPass,
                           bool isLastRenderPass) {
        this->physicalDevice = physicalDevice;
        this->logicalDevice = logicalDevice;
        this->swapChainImageFormat = swapChainImageFormat;
        this->isFirstRenderPass = isFirstRenderPass;
        this->isLastRenderPass = isLastRenderPass;

        // not used in ImGui
        defaultClearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        defaultClearValues[1].depthStencil = {1.0f, 0};
    }

    // #endregion

    // #region Destructors

    RenderPass::~RenderPass() {
        cout << "Destroy Render Pass" << endl;

        vkDestroyRenderPass(logicalDevice->GetVulkanDevice(), vulkanRenderPass, nullptr);
    }

    void RenderPass::Begin(VkCommandBuffer vulkanCommandBuffer, VkFramebuffer vulkanFrameBuffer, VkExtent2D extent) {
        VkRenderPassBeginInfo renderPassBeginInfo = {};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = vulkanRenderPass;
        renderPassBeginInfo.framebuffer = vulkanFrameBuffer;

        renderPassBeginInfo.renderArea.offset = {0, 0};
        renderPassBeginInfo.renderArea.extent = extent;

        SetClearValues(renderPassBeginInfo);

        vkCmdBeginRenderPass(vulkanCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    // perform additional rendering from render pass if required
    // is overridden by the ImGui Render pass to render its data
    // could be done better
    void RenderPass::Render(VkCommandBuffer vulkanCommandBuffer) {}

    void RenderPass::End(VkCommandBuffer vulkanCommandBuffer) {
        vkCmdEndRenderPass(vulkanCommandBuffer);
    }



    // #endregion

} // KTXCompressor