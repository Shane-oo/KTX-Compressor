//
// Created by ShaneMonck on 3/09/2024.
//

#include "ImageTexture.h"

namespace KTXCompressor {
    // #region Private Methods

    void ImageTexture::AddAlphaChannelToImage(unique_ptr<unsigned char[]> &pixels, uint32_t width, uint32_t height,
                                              int channels) {
        size_t original_size = width * height * channels;
        size_t new_channels = channels + 1;
        size_t new_size = width * height * new_channels;

        unique_ptr<unsigned char[]> new_pixels(new unsigned char[new_size]);

        // Iterate through the original pixels and copy them, adding an alpha value
        for (size_t i = 0, j = 0; i < original_size; i += channels, j += new_channels) {
            // Copy existing channels
            std::memcpy(&new_pixels[j], &pixels[i], channels);
            // Add alpha channel (fully opaque)
            new_pixels[j + channels] = 255;
        }

        // Replace the old pixel data with the new one
        pixels.swap(new_pixels);
    }

    void ImageTexture::LoadImageForFile(const string &fileName) {
        Image_Input = ImageInput::open(fileName);
        if (!Image_Input) {
            throw runtime_error("Could Not Open File With Name: " + fileName);
        }
        // Always go to first sub image and first MipLevel
        Image_Input->seek_subimage(0, 0);

        auto imageSpecs = Image_Input->spec();

        auto width = static_cast<uint32_t>(imageSpecs.width);
        auto height = static_cast<uint32_t>(imageSpecs.height);
        int channels = imageSpecs.nchannels;
        auto originalImageSize = width * height * channels;

        auto pixels = std::unique_ptr<unsigned char[]>(new unsigned char[originalImageSize]);
        Image_Input->read_image(0, 0, 0, channels, TypeDesc::UINT8, &pixels[0]);

        if (channels == 3) {
            // Missing Alpha
            // GPU's do not guarantee to support images without an alpha channel, so you must always have one
            AddAlphaChannelToImage(pixels, width, height, channels);
        }

        Image_Input->close();

        CreateImage(width, height, pixels.get());
        sampler = CreateTextureSampler();
    }

    // #endregion


    // #region Constructors

    ImageTexture::ImageTexture(const string &fileName, LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice)
            : Texture(logicalDevice, physicalDevice) {
        this->name = fileName;

        LoadImageForFile(fileName);
    }

    // #endregion

    // #region Destructors

    ImageTexture::~ImageTexture() {
        cout << "Destroying Image Texture " << name << endl;

        vkDestroySampler(logicalDevice->GetVulkanDevice(), sampler, nullptr);
    }

    // #endregion

} // KTXCompressor