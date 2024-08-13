//
// Created by shane on 28/07/2024.
//

#include "SimpleTriangleGraphicsPipeline.h"
#include "SimpleTriangleShader.h"

namespace KTXCompressor {


    // #region Protected Methods
    Shader *SimpleTriangleGraphicsPipeline::CreateShader() {
        return new SimpleTriangleShader(physicalDevice, logicalDevice);
    }


    void SimpleTriangleGraphicsPipeline::SetRasterizationStateCreateInfo(
            VkPipelineRasterizationStateCreateInfo &rasterizationStateCreateInfo) {
        rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
        rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizationStateCreateInfo.lineWidth = 1.0f;
        rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
        rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f; // Optional
        rasterizationStateCreateInfo.depthBiasClamp = 0.0f; // Optional
        rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f; // Optional
    }

    // #endregion

    // #region Constructors 


    SimpleTriangleGraphicsPipeline::SimpleTriangleGraphicsPipeline(PhysicalDevice *physicalDevice,
                                                                   LogicalDevice *logicalDevice,
                                                                   SwapChain *swapChain,
                                                                   uint32_t graphicsFamilyIndex)
            : GraphicsPipeline(physicalDevice, logicalDevice, swapChain, graphicsFamilyIndex) {
        Init();
    }



    // #endregion

    // #region Public Methods

    // #endregion

} // KTXCompressor