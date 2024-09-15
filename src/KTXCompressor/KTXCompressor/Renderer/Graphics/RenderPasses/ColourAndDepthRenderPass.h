//
// Created by ShaneMonck on 15/09/2024.
//

#ifndef KTX_COMPRESSOR_COLOURANDDEPTHRENDERPASS_H
#define KTX_COMPRESSOR_COLOURANDDEPTHRENDERPASS_H

#include "RenderPass.h"

namespace KTXCompressor {

    class ColourAndDepthRenderPass : public RenderPass {

    public:
        ColourAndDepthRenderPass(PhysicalDevice *physicalDevice, LogicalDevice *logicalDevice,
                                 VkFormat swapChainImageFormat,
                                 bool isFirstRenderPass,
                                 bool isLastRenderPass);

    protected:
        VkRenderPass CreateVulkanRenderPass() override;
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_COLOURANDDEPTHRENDERPASS_H
