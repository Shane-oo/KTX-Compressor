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

        auto channels = imageSpecs.nchannels;
        auto size = imageSpecs.width * imageSpecs.height * channels;

        auto pixels = unique_ptr<unsigned char[]>(
                new unsigned char[size]
        );
        ImageInput->read_image(0, 0, 0, channels, TypeDesc::UINT8, pixels.get());
        VkDeviceSize deviceSize = imageSpecs.width * imageSpecs.height * imageSpecs.nchannels;

        VkImageCreateInfo imageCreateInfo = {};
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        imageCreateInfo.extent.width = static_cast<uint32_t>(imageSpecs.width);
        imageCreateInfo.extent.height = static_cast<uint32_t>(imageSpecs.height);
        imageCreateInfo.extent.depth = 1;
        imageCreateInfo.mipLevels = 1;
        imageCreateInfo.arrayLayers = 1;
        imageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
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
                                             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    }

    // #endregion


    // #region Constructors

    Texture::Texture(LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice, string fileName) {
        this->bufferUtil = new BufferUtil(logicalDevice, physicalDevice);
        LoadImageForFile(fileName);
    }

    // #endregion

    // #region Destructors

    Texture::~Texture() {

    }

    // #endregion

} // KTXCompressor