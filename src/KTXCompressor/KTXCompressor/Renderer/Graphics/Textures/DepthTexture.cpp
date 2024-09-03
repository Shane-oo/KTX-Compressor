//
// Created by ShaneMonck on 3/09/2024.
//

#include "DepthTexture.h"

namespace KTXCompressor {
    DepthTexture::DepthTexture(VkExtent2D extent, LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice)
            : Texture(logicalDevice,
                      physicalDevice) {
        VkFormat depthFormat = physicalDevice->FindSupportedFormat(
                {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                VK_IMAGE_TILING_OPTIMAL,
                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);


        CreateImageWithoutPixels(extent.width, extent.height, depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

        page 226
        Explicitly transitioning the depth image
    }
} // KTXCompressor