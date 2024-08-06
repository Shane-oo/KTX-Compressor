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

    VkPipelineLayout SimpleTriangleShader::CreatePipelineLayout() {
        VkPipelineLayout pipelineLayout;

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.setLayoutCount = 0; // Optional
        pipelineLayoutCreateInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutCreateInfo.pPushConstantRanges = nullptr; // Optional
        pipelineLayoutCreateInfo.pNext = nullptr;
        pipelineLayoutCreateInfo.flags = VK_PIPELINE_LAYOUT_CREATE_INDEPENDENT_SETS_BIT_EXT;

        VkResult pipelineLayoutCreateResult = vkCreatePipelineLayout(vulkanDevice,
                                                                     &pipelineLayoutCreateInfo,
                                                                     nullptr,
                                                                     &pipelineLayout);

        if (pipelineLayoutCreateResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Pipeline Layout for SimpleTriangleShader");
        }

        cout << "Successfully Created Pipeline Layout for SimpleTriangleShader" << endl;

        return pipelineLayout;
    }

    // #endregion

} // KTXCompressor