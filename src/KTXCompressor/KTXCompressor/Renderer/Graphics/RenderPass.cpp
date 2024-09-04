//
// Created by shane on 28/07/2024.
//

#include "RenderPass.h"

namespace KTXCompressor {

    // #region Private Methods

    VkRenderPass RenderPass::CreateVulkanRenderPass(VkFormat swapChainImageFormat) {
        VkAttachmentDescription colorAttachmentDescription = {};
        colorAttachmentDescription.format = swapChainImageFormat;
        colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;

        colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // clear the values to a constant at the start
        colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // rendered contents will be stored in memory to be read later 

        colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentReference = {};
        colorAttachmentReference.attachment = 0;  // index of colorAttachmentDescription
        colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


        VkAttachmentDescription depthAttachmentDescription = {};
        depthAttachmentDescription.format = physicalDevice->FindDepthFormat();
        depthAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentReference = {};
        depthAttachmentReference.attachment = 1;
        depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        // SubPass
        VkSubpassDescription subpassDescription = {};
        subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; // graphics subpass
        subpassDescription.colorAttachmentCount = 1;
        subpassDescription.pColorAttachments = &colorAttachmentReference;
        subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;

        VkSubpassDependency subpassDependency = {};
        subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependency.dstSubpass = 0; // refers to our subpass, which is first and only one
        // need to wait for the swap chain to finish reading from the image before we can access it
        subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
                                         | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpassDependency.srcAccessMask = 0;
        subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
                                         | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                                          VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;


        array<VkAttachmentDescription, 2> attachmentDescriptions = {colorAttachmentDescription,
                                                                    depthAttachmentDescription};
        VkRenderPassCreateInfo renderPassCreateInfo = {};
        renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size());
        renderPassCreateInfo.pAttachments = attachmentDescriptions.data();
        renderPassCreateInfo.subpassCount = 1;
        renderPassCreateInfo.pSubpasses = &subpassDescription;
        renderPassCreateInfo.dependencyCount = 1;
        renderPassCreateInfo.pDependencies = &subpassDependency;

        VkRenderPass renderPass;
        VkResult createRenderPassResult = vkCreateRenderPass(logicalDevice->GetVulkanDevice(),
                                                             &renderPassCreateInfo,
                                                             nullptr,
                                                             &renderPass);
        if (createRenderPassResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Render Pass");
        }

        cout << "Successfully Created Render Pass" << endl;

        return renderPass;
    }
    // #endregion

    // #region Constructors

    RenderPass::RenderPass(PhysicalDevice *physicalDevice,
                           LogicalDevice *logicalDevice,
                           VkFormat swapChainImageFormat) {
        this->physicalDevice = physicalDevice;
        this->logicalDevice = logicalDevice;
        vulkanRenderPass = CreateVulkanRenderPass(swapChainImageFormat);
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

        array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());;
        renderPassBeginInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(vulkanCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void RenderPass::End(VkCommandBuffer vulkanCommandBuffer) {
        vkCmdEndRenderPass(vulkanCommandBuffer);
    }


    // #endregion

} // KTXCompressor