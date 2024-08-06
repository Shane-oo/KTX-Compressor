//
// Created by shane on 19/07/2024.
//

#ifndef KTXCOMPRESSOR_IMAGEVIEW_H
#define KTXCOMPRESSOR_IMAGEVIEW_H

#include <vulkan/vulkan_core.h>
#include "../../Common.h"

namespace KTXCompressor {

    class ImageView {
    public:
        ImageView(VkDevice vulkanDevice, VkImage vulkanImage, VkFormat imageFormat);

        ~ImageView();

    private:
        VkImageView vulkanImageView;
        VkDevice vulkanDevice;

    private:
        VkImageView CreateVulkanImageView(VkImage vulkanImage, VkFormat imageFormat);

    public:
        VkImageView GetVulkanImageView() {
            return vulkanImageView;
        }
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_IMAGEVIEW_H
