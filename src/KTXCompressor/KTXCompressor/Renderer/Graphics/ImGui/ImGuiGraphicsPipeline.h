//
// Created by ShaneMonck on 4/09/2024.
//

#ifndef KTX_COMPRESSOR_IMGUIGRAPHICSPIPELINE_H
#define KTX_COMPRESSOR_IMGUIGRAPHICSPIPELINE_H

#include "../GraphicsPipeline.h"

namespace KTXCompressor {

    class ImGuiGraphicsPipeline : public GraphicsPipeline {
    public:
        ImGuiGraphicsPipeline(Window *window,
                              PhysicalDevice *physicalDevice,
                              LogicalDevice *logicalDevice,
                              SwapChain *swapChain,
                              uint32_t graphicsFamilyIndex);

    protected:
        Shader *CreateShader() override;

        void
        SetRasterizationStateCreateInfo(VkPipelineRasterizationStateCreateInfo &rasterizationStateCreateInfo) override;
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_IMGUIGRAPHICSPIPELINE_H
