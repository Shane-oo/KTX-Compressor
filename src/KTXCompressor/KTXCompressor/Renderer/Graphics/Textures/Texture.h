//
// Created by ShaneMonck on 15/08/2024.
//

#ifndef KTX_COMPRESSOR_TEXTURE_H
#define KTX_COMPRESSOR_TEXTURE_H

#include "../../../Common.h"
#include "vulkan/vulkan_core.h"
#include "../../Utils/BufferUtil.h"
#include "../../Presentation/ImageView.h"
#include <OpenImageIO/imageio.h>

using namespace OIIO;

namespace KTXCompressor {

    class Texture {
    public:
        Texture(LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice, const string &fileName);

        ~Texture();

    private:
        LogicalDevice *logicalDevice;
        PhysicalDevice *physicalDevice;
        string name;
        BufferUtil *bufferUtil;
        unique_ptr<ImageInput> ImageInput;
        VkImage vulkanImage = nullptr;
        VkDeviceMemory vulkanImageMemory;
        ImageView *textureImageView;
        VkSampler textureSampler;

        void LoadImageForFile(const string &fileName);

        void LoadKtx2File(const string &fileName);

        void CreateImage(uint32_t width,
                         uint32_t height,
                         const void *pixels);

        VkSampler CreateTextureSampler();

        static void AddAlphaChannelToImage(unique_ptr<unsigned char[]> &pixels,
                                           uint32_t width,
                                           uint32_t height,
                                           int channels);

    public:
        ImageView *GetImageView() {
            return textureImageView;
        }

        VkSampler GetSampler() {
            return textureSampler;
        }

    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_TEXTURE_H
