//
// Created by shane on 28/07/2024.
//

#include "SimpleTriangleGraphicsPipeline.h"
#include "SimpleTriangleShader.h"
#include "../RenderPasses/ColourAndDepthRenderPass.h"

namespace KTXCompressor {


    // #region Protected Methods
    Shader *SimpleTriangleGraphicsPipeline::CreateShader() {
        return new SimpleTriangleShader(physicalDevice, logicalDevice, renderPass, swapChain->GetExtent(), imagePath);
    }


    // #endregion

    // #region Constructors 


    SimpleTriangleGraphicsPipeline::SimpleTriangleGraphicsPipeline(PhysicalDevice *physicalDevice,
                                                                   LogicalDevice *logicalDevice,
                                                                   SwapChain *swapChain,
                                                                   bool isFirstToRender,
                                                                   bool isLastToRender,
                                                                   string imagePath)
            : GraphicsPipeline(physicalDevice,
                               logicalDevice,
                               swapChain,
                               isFirstToRender,
                               isLastToRender,
                               true) {
        this->imagePath = imagePath;
        Init();
    }

    RenderPass *SimpleTriangleGraphicsPipeline::CreateRenderPass() {
        return new ColourAndDepthRenderPass(physicalDevice,
                                            logicalDevice,
                                            swapChain->GetImageFormat(),
                                            isFirstToRender,
                                            isLastToRender);
    }



    // #endregion

    // #region Public Methods

    // #endregion

} // KTXCompressor