//
// Created by ShaneMonck on 15/09/2024.
//

#include <imgui_impl_vulkan.h>
#include "ImGuiRenderPass.h"

namespace KTXCompressor {
    // #region Protected Methods

    VkRenderPass ImGuiRenderPass::CreateVulkanRenderPass() {
        if (!isLastRenderPass) {
            throw runtime_error("Im Gui Render Pass Must be Last!");
        }

        VkAttachmentDescription colorAttachmentDescription = {};
        colorAttachmentDescription.format = swapChainImageFormat;
        colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;

        colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

        colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        // previous rendering should be 
        colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // last render pass

        VkAttachmentReference colorAttachmentReference = {};
        colorAttachmentReference.attachment = 0;
        colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpassDescription = {};
        subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; // graphics subpass
        subpassDescription.colorAttachmentCount = 1;
        subpassDescription.pColorAttachments = &colorAttachmentReference;

        VkSubpassDependency subpassDependency = {};
        subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependency.dstSubpass = 0;
        subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassCreateInfo = {};
        renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.attachmentCount = 1;
        renderPassCreateInfo.pAttachments = &colorAttachmentDescription;
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
            throw runtime_error("Failed to create Render Pass for ImGui");
        }

        cout << "Successfully Created Render Pass For ImGui" << endl;

        return renderPass;
    }

    void ImGuiRenderPass::SetClearValues(VkRenderPassBeginInfo &renderPassBeginInfo) {
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &vulkanClearValue;
    }



    // #endregion

    // #region Constructors

    ImGuiRenderPass::ImGuiRenderPass(PhysicalDevice *physicalDevice,
                                     LogicalDevice *logicalDevice,
                                     VkFormat swapChainImageFormat,
                                     bool isFirstRenderPass,
                                     bool isLastRenderPass) : RenderPass(physicalDevice,
                                                                         logicalDevice,
                                                                         swapChainImageFormat,
                                                                         isFirstRenderPass,
                                                                         isLastRenderPass) {
        // Set vulkanClearValue for color of ImGui
        auto clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.0f);
        memcpy(&vulkanClearValue.color.float32[0], &clearColor, 4 * sizeof(float));

        Init();
    }


    // #endregion

    // #region Public Methods

    void ImGuiRenderPass::Render(VkCommandBuffer vulkanCommandBuffer) {
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), vulkanCommandBuffer);
    }

    // #endregion

} // KTXCompressor