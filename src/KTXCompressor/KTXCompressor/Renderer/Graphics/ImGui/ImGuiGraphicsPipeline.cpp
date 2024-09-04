//
// Created by ShaneMonck on 4/09/2024.
//

#include "ImGuiGraphicsPipeline.h"

namespace KTXCompressor {
    // #region Constructors

    ImGuiGraphicsPipeline::ImGuiGraphicsPipeline(PhysicalDevice *physicalDevice,
                                                 LogicalDevice *logicalDevice,
                                                 SwapChain *swapChain,
                                                 uint32_t graphicsFamilyIndex)
            : GraphicsPipeline(physicalDevice,
                               logicalDevice,
                               swapChain,
                               graphicsFamilyIndex) {

    }

    // #endregion
} // KTXCompressor