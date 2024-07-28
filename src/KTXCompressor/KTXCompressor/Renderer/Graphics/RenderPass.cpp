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

        // SubPass
        VkSubpassDescription subpassDescription = {};
        subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; // graphics subpass
        subpassDescription.colorAttachmentCount = 1;
        subpassDescription.pColorAttachments = &colorAttachmentReference;


        VkRenderPassCreateInfo renderPassCreateInfo = {};
        renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.attachmentCount = 1;
        renderPassCreateInfo.pAttachments = &colorAttachmentDescription;
        renderPassCreateInfo.subpassCount = 1;
        renderPassCreateInfo.pSubpasses = &subpassDescription;

        VkRenderPass renderPass;
        VkResult createRenderPassResult = vkCreateRenderPass(vulkanDevice,
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

    RenderPass::RenderPass(VkDevice vulkanDevice, VkFormat swapChainImageFormat) {
        this->vulkanDevice = vulkanDevice;
        vulkanRenderPass = CreateVulkanRenderPass(swapChainImageFormat);
    }

    // #endregion

    // #region Destructors

    RenderPass::~RenderPass() {
        cout << "Destroy Render Pass" << endl;

        vkDestroyRenderPass(vulkanDevice, vulkanRenderPass, nullptr);
    }


    // #endregion

} // KTXCompressor