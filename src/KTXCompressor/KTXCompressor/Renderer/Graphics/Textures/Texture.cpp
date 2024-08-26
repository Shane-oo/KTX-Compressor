//
// Created by ShaneMonck on 15/08/2024.
//

#include "Texture.h"

namespace KTXCompressor {

    // #region Private Methods

    void Texture::LoadImageForFile(const string &fileName) {
        ImageInput = ImageInput::open(fileName);
        if (!ImageInput) {
            throw runtime_error("Could Not Open File With Name: " + fileName);
        }
        // Always go to first sub image and first MipLevel
        ImageInput->seek_subimage(0, 0);

        auto imageSpecs = ImageInput->spec();

        auto channels = 4;  // (R,G,B,A) //imageSpecs.nchannels;
        VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;

        auto width = static_cast<uint32_t>(imageSpecs.width);
        auto height = static_cast<uint32_t>(imageSpecs.height);
        auto size = width * height * channels;


        auto pixels = unique_ptr<unsigned char[]>(
                new unsigned char[size]
        );
        ImageInput->read_image(0, 0, 0, channels, TypeDesc::UINT8, pixels.get());
        VkDeviceSize deviceSize = size;


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
        imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageCreateInfo.flags = 0; //Optional

        this->bufferUtil->CreateAndFillImage(pixels.get(),
                                             deviceSize,
                                             imageCreateInfo,
                                             vulkanImage,
                                             vulkanImageMemory,
                                             format,
                                             width,
                                             height,
                                             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
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


    // #region Constructors

    Texture::Texture(LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice, const string &fileName) {
        this->logicalDevice = logicalDevice;
        this->physicalDevice = physicalDevice;
        this->bufferUtil = new BufferUtil(logicalDevice, physicalDevice);

        this->name = fileName;
        LoadImageForFile(fileName);

        if (vulkanImage) {
            textureImageView = new ImageView(logicalDevice->GetVulkanDevice(), vulkanImage, VK_FORMAT_R8G8B8A8_SRGB);
            textureSampler = CreateTextureSampler();
        }
    }

    // #endregion

    // #region Destructors

    Texture::~Texture() {
        cout << "Destroying " << name << endl;
        vkDestroySampler(logicalDevice->GetVulkanDevice(), textureSampler, nullptr);

        delete textureImageView;

        vkDestroyImage(logicalDevice->GetVulkanDevice(), vulkanImage, nullptr);
        vkFreeMemory(logicalDevice->GetVulkanDevice(), vulkanImageMemory, nullptr);
    }



    // #endregion

} // KTXCompressor