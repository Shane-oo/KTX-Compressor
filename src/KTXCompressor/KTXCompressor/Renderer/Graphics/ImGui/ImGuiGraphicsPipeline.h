//
// Created by ShaneMonck on 4/09/2024.
//

#ifndef KTX_COMPRESSOR_IMGUIGRAPHICSPIPELINE_H
#define KTX_COMPRESSOR_IMGUIGRAPHICSPIPELINE_H

#include "../GraphicsPipeline.h"

namespace KTXCompressor {

    class ImGuiGraphicsPipeline : public GraphicsPipeline {
    public:
        ImGuiGraphicsPipeline(PhysicalDevice *physicalDevice,
                              LogicalDevice *logicalDevice,
                              SwapChain *swapChain,
                              uint32_t graphicsFamilyIndex,
                              int index);
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_IMGUIGRAPHICSPIPELINE_H
