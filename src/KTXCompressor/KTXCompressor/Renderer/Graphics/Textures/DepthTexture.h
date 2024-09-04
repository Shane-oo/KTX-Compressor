//
// Created by ShaneMonck on 3/09/2024.
//

#ifndef KTX_COMPRESSOR_DEPTHTEXTURE_H
#define KTX_COMPRESSOR_DEPTHTEXTURE_H

#include "../../../Common.h"
#include "Texture.h"

namespace KTXCompressor {

    class DepthTexture : public Texture {
    public:
        DepthTexture(VkExtent2D extent, LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice);
        
        ~DepthTexture();

    public:
        VkSampler GetSampler() override {
            return nullptr;
        }

        static bool HasStencilComponent(VkFormat format) {
            return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format ==
                                                             VK_FORMAT_D24_UNORM_S8_UINT;
        }
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_DEPTHTEXTURE_H
