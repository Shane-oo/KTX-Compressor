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
        VkImage vulkanImage;
        VkDeviceMemory vulkanImageMemory;
        ImageView *textureImageView;
        VkSampler textureSampler;


        void LoadImageForFile(const string &fileName);

        VkSampler CreateTextureSampler();
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_TEXTURE_H
