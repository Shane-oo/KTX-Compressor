//
// Created by shane on 28/07/2024.
//

#include "GraphicsPipeline.h"

namespace KTXCompressor {

    // #region Constructors

    GraphicsPipeline::GraphicsPipeline(VkDevice device) {
        vulkanDevice = device;
    }
    // #endregion

    // #region Destructors 

    GraphicsPipeline::~GraphicsPipeline() {

    }

    // #endregion

    // #region Protected Methods

    void GraphicsPipeline::Init() {
        shader = CreateShader();

        VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {};
        vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertexShaderStageInfo.module = shader->GetVertexShaderModule();
        vertexShaderStageInfo.pName = shader->GetVertexEntryPointName();

        VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {};
        fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragmentShaderStageInfo.module = shader->GetFragmentShaderModule();
        fragmentShaderStageInfo.pName = shader->GetFragmentEntryPointName();

        VkPipelineShaderStageCreateInfo shaderStages[]{
                vertexShaderStageInfo,
                fragmentShaderStageInfo
        };
        
        




        // dont think ill need the shader anymore??
        // or the destroy of the shader modules should go at the end of the shader constructor
        delete shader;
    }


    // #endregion

} // KTXCompressor