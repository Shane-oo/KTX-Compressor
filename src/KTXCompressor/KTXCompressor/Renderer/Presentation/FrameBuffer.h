//
// Created by shane on 6/08/2024.
//

#ifndef KTXCOMPRESSOR_FRAMEBUFFER_H
#define KTXCOMPRESSOR_FRAMEBUFFER_H

#include <vulkan/vulkan_core.h>
#include "../../Common.h"

namespace KTXCompressor {

    class FrameBuffer {
    public:
        FrameBuffer(VkDevice vulkanDevice,
                    vector<VkImageView> attachments,
                    VkRenderPass vulkanRenderPass,
                    VkExtent2D extent);

        ~FrameBuffer();

    private:
        VkFramebuffer vulkanFrameBuffer;
        VkDevice vulkanDevice;

        VkFramebuffer CreateFrameBuffer(vector<VkImageView> attachments,
                                        VkRenderPass vulkanRenderPass,
                                        VkExtent2D extent);
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_FRAMEBUFFER_H
