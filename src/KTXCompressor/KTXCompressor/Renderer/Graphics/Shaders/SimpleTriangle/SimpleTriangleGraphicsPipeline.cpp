//
// Created by shane on 28/07/2024.
//

#include "SimpleTriangleGraphicsPipeline.h"
#include "SimpleTriangleShader.h"

namespace KTXCompressor {


    // #region Protected Methods
    Shader *SimpleTriangleGraphicsPipeline::CreateShader() {
        return new SimpleTriangleShader(vulkanDevice);
    }

    // #endregion

    // #region Constructors 


    SimpleTriangleGraphicsPipeline::SimpleTriangleGraphicsPipeline(VkDevice device) : GraphicsPipeline(device) {
        Init();
    }

    // #endregion

} // KTXCompressor