//
// Created by ShaneMonck on 15/08/2024.
//

#include "Texture.h"

namespace KTXCompressor {

    // #region Private Methods

    void Texture::CreateImage(uint32_t width,
                              uint32_t height,
                              const void *pixels) {
        VkDeviceSize imageSize = width * height * 4; // RGBA

        VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;

        VkImageCreateInfo imageCreateInfo = GetImageCreateInfo(width,
                                                               height,
                                                               format,
                                                               VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                                               VK_IMAGE_USAGE_SAMPLED_BIT);

        bufferUtil->CreateAndFillImage(pixels,
                                       imageSize,
                                       imageCreateInfo,
                                       vulkanImage,
                                       vulkanImageMemory,
                                       format,
                                       width,
                                       height);

        imageView = new ImageView(logicalDevice->GetVulkanDevice(), vulkanImage, format, VK_IMAGE_ASPECT_COLOR_BIT);
    }

    VkImageCreateInfo Texture::GetImageCreateInfo(uint32_t width, uint32_t height, const VkFormat &format,
                                                  VkImageUsageFlags imageUsageFlags) {
        VkImageCreateInfo imageCreateInfo = {};
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        imageCreateInfo.extent.width = width;
        imageCreateInfo.extent.height = height;
        imageCreateInfo.extent.depth = 1;
        imageCreateInfo.mipLevels = 1;
        imageCreateInfo.arrayLayers = 1;
        imageCreateInfo.format = format;
        imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageCreateInfo.usage = imageUsageFlags;
        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageCreateInfo.flags = 0; //Optional

        return imageCreateInfo;
    }

    VkSampler Texture::CreateTextureSampler() {
        VkSamplerCreateInfo samplerCreateInfo = {};
        samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
        samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
        samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.anisotropyEnable = VK_TRUE;
        samplerCreateInfo.maxAnisotropy = physicalDevice->GetMaxSamplerAnisotropy();
        samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerCreateInfo.unnormalizedCoordinates = VK_FALSE; // texels addressed using [0,1) range
        samplerCreateInfo.compareEnable = VK_FALSE;
        samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        // MipMapping
        samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerCreateInfo.mipLodBias = 0.0f;
        samplerCreateInfo.minLod = 0.0f;
        samplerCreateInfo.maxLod = 0.0f;

        VkSampler sampler;
        VkResult createSamplerResult = vkCreateSampler(logicalDevice->GetVulkanDevice(),
                                                       &samplerCreateInfo,
                                                       nullptr,
                                                       &sampler);
        if (createSamplerResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Texture Sampler");
        }

        return sampler;
    }

    // #endregion

    // #region Protected Methods
    
    void Texture::CleanUpImageView() {
        delete bufferUtil;
        delete imageView;

        vkDestroyImage(logicalDevice->GetVulkanDevice(), vulkanImage, nullptr);
        vkFreeMemory(logicalDevice->GetVulkanDevice(), vulkanImageMemory, nullptr);
    }

    // #endregion


    // #region Constructors

    Texture::Texture(LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice) {
        this->logicalDevice = logicalDevice;
        this->physicalDevice = physicalDevice;
        this->bufferUtil = new BufferUtil(logicalDevice, physicalDevice);
    }

    // #endregion

    // #region Destructors


    // #endregion

} // KTXCompressor