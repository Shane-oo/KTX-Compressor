//
// Created by ShaneMonck on 15/08/2024.
//

#ifndef KTX_COMPRESSOR_TEXTURE_H
#define KTX_COMPRESSOR_TEXTURE_H

#include "../../../Common.h"
#include "vulkan/vulkan_core.h"
#include "../../Utils/BufferUtil.h"
#include "../../Presentation/ImageView.h"


namespace KTXCompressor {

    class Texture {
    public:
        Texture(LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice);

        ~Texture();

        virtual VkSampler GetSampler() = 0;

    protected:
        LogicalDevice *logicalDevice;
        PhysicalDevice *physicalDevice;
        BufferUtil *bufferUtil;
        VkImage vulkanImage;
        VkDeviceMemory vulkanImageMemory;
        ImageView *imageView;

        VkImageCreateInfo GetImageCreateInfo(uint32_t width,
                           uint32_t height,
                           const VkFormat &format,
                           VkImageUsageFlags imageUsageFlags);

        void CreateImage(uint32_t width,
                         uint32_t height,
                         const void *pixels);

        VkSampler CreateTextureSampler();

    private:


    public:
        ImageView *GetImageView() {
            return imageView;
        }

    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_TEXTURE_H
