//
// Created by ShaneMonck on 15/08/2024.
//

#ifndef KTX_COMPRESSOR_TEXTURE_H
#define KTX_COMPRESSOR_TEXTURE_H

#include "../../../Common.h"
#include "vulkan/vulkan_core.h"
#include "../../Utils/BufferUtil.h"
#include <OpenImageIO/imageio.h>

using namespace OIIO;

namespace KTXCompressor {

    class Texture {
    public:
        Texture(LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice, const string& fileName);

        ~Texture();

    private:
        LogicalDevice *logicalDevice;
        string name;
        BufferUtil *bufferUtil;
        unique_ptr<ImageInput> ImageInput;
        VkImage vulkanImage;
        VkDeviceMemory vulkanImageMemory;

        void LoadImageForFile(const string &fileName);
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_TEXTURE_H
