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

    public:
        VkSampler GetSampler() override {
            return nullptr;
        }
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_DEPTHTEXTURE_H
