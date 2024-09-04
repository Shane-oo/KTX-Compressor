//
// Created by shane on 6/08/2024.
//

#include "FrameBuffer.h"

namespace KTXCompressor {


    // #region Private Methods

    VkFramebuffer FrameBuffer::CreateFrameBuffer(vector<VkImageView> attachments,
                                                 VkRenderPass vulkanRenderPass,
                                                 VkExtent2D extent) {
        VkFramebufferCreateInfo framebufferCreateInfo = {};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.renderPass = vulkanRenderPass;
        framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferCreateInfo.pAttachments = attachments.data();
        framebufferCreateInfo.width = extent.width;
        framebufferCreateInfo.height = extent.height;
        framebufferCreateInfo.layers = 1;

        VkFramebuffer framebuffer;
        VkResult createFrameBufferResult = vkCreateFramebuffer(vulkanDevice,
                                                               &framebufferCreateInfo,
                                                               nullptr,
                                                               &framebuffer);
        if (createFrameBufferResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Frame Buffer");
        }

        cout << "Successfully Created Frame Buffer" << endl;

        return framebuffer;
    }

    // #endregion


    // #region Constructors

    FrameBuffer::FrameBuffer(VkDevice vulkanDevice,
                             vector<VkImageView> attachments,
                             VkRenderPass vulkanRenderPass,
                             VkExtent2D extent) {
        this->vulkanDevice = vulkanDevice;
        vulkanFrameBuffer = CreateFrameBuffer(attachments, vulkanRenderPass, extent);
    }

    // #endregion

    // #region Destructors

    FrameBuffer::~FrameBuffer() {
        cout << "Destroy Frame Buffer" << endl;

        vkDestroyFramebuffer(vulkanDevice, vulkanFrameBuffer, nullptr);
    }

    // #endregion
} // KTXCompressor