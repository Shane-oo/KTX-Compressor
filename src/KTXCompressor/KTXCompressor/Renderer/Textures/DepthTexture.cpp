//
// Created by ShaneMonck on 3/09/2024.
//

#include "DepthTexture.h"

namespace KTXCompressor {

    // #region Constructors

    DepthTexture::DepthTexture(VkExtent2D extent, LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice)
            : Texture(logicalDevice,
                      physicalDevice) {
        VkFormat depthFormat = physicalDevice->FindDepthFormat();

        VkImageCreateInfo imageCreateInfo = GetImageCreateInfo(extent.width,
                                                               extent.height,
                                                               depthFormat,
                                                               VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

        bufferUtil->CreateImage(imageCreateInfo,
                                vulkanImage,
                                vulkanImageMemory,
                                depthFormat,
                                true,
                                VK_IMAGE_LAYOUT_UNDEFINED,
                                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        imageView = new ImageView(logicalDevice->GetVulkanDevice(),
                                  vulkanImage,
                                  depthFormat,
                                  VK_IMAGE_ASPECT_DEPTH_BIT);
    }

    // #endregion

    // #region Destructors

    DepthTexture::~DepthTexture() {
        cout << "Destroying Depth Texture" << endl;

        CleanUpImageView();
    }

    // #endregion


} // KTXCompressor