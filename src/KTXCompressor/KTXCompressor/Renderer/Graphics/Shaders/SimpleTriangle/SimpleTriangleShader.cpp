//
// Created by shane on 28/07/2024.
//

#include "SimpleTriangleShader.h"

namespace KTXCompressor {

    // #region Constructors

    SimpleTriangleShader::SimpleTriangleShader(VkDevice vulkanDevice)
            : Shader(vulkanDevice, "simple_triangle.vert.spv", "simple_triangle.frag.spv") {
    }

    // #endregion

    // #region Public Methods

    const char *SimpleTriangleShader::GetVertexEntryPointName() {
        return "main";
    }

    const char *SimpleTriangleShader::GetFragmentEntryPointName() {
        return "main";
    }

    // #endregion

} // KTXCompressor