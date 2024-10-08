//
// Created by shane on 28/07/2024.
//

#ifndef KTXCOMPRESSOR_SIMPLETRIANGLEGRAPHICSPIPELINE_H
#define KTXCOMPRESSOR_SIMPLETRIANGLEGRAPHICSPIPELINE_H

#include "../GraphicsPipeline.h"
#include "../../Presentation/SwapChain.h"

namespace KTXCompressor {

    class SimpleTriangleGraphicsPipeline : public GraphicsPipeline {

    public:
        SimpleTriangleGraphicsPipeline(
                PhysicalDevice *physicalDevice,
                LogicalDevice *logicalDevice,
                SwapChain *swapChain,
                bool isFirstToRender,
                bool isLastToRender);

    protected:
        Shader *CreateShader() override;
        
        RenderPass *CreateRenderPass() override;

     
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_SIMPLETRIANGLEGRAPHICSPIPELINE_H
