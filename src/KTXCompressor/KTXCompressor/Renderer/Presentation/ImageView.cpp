//
// Created by shane on 19/07/2024.
//

#include "ImageView.h"

namespace KTXCompressor {
    // #region Private Methods

    VkImageView ImageView::CreateVulkanImageView(VkImage vulkanImage, VkFormat imageFormat) {
        VkImageViewCreateInfo imageViewCreateInfo{};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image = vulkanImage;
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = imageFormat;

        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        // our images will be used as colour targets without any mip-mapping levels or multiple layers for now
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        VkResult createImageViewResult = vkCreateImageView(vulkanDevice,
                                                           &imageViewCreateInfo,
                                                           nullptr,
                                                           &imageView);

        if (createImageViewResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Image View");
        }

        cout << "Successfully Created Image View" << endl;

        return imageView;
    }

    // #endregion

    // #region Constructors

    ImageView::ImageView(VkDevice vulkanDevice, VkImage vulkanImage, VkFormat imageFormat) {
        this->vulkanDevice = vulkanDevice;
        vulkanImageView = CreateVulkanImageView(vulkanImage, imageFormat);
    }

    // #endregion

    // #region Destructors

    ImageView::~ImageView() {
        cout << "Destroy Image View" << endl;
        vkDestroyImageView(vulkanDevice, vulkanImageView, nullptr);
    }

    // #endregion

} // KTXCompressor