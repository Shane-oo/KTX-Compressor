//
// Created by shane on 28/07/2024.
//

#ifndef KTXCOMPRESSOR_SIMPLETRIANGLEGRAPHICSPIPELINE_H
#define KTXCOMPRESSOR_SIMPLETRIANGLEGRAPHICSPIPELINE_H

#include "../../GraphicsPipeline.h"
#include "../../../Presentation/SwapChain.h"

namespace KTXCompressor {

    class SimpleTriangleGraphicsPipeline : public GraphicsPipeline {

    public:
        SimpleTriangleGraphicsPipeline(VkDevice device, SwapChain* swapChain);

    protected:
        Shader *CreateShader() override;
        
        void SetRasterizationStateCreateInfo(VkPipelineRasterizationStateCreateInfo &rasterizationStateCreateInfo) override;
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_SIMPLETRIANGLEGRAPHICSPIPELINE_H
