//
// Created by shane on 28/07/2024.
//

#include "GraphicsPipeline.h"
#include "../Synchronization/Synchronization.h"

namespace KTXCompressor {

    // #region Private Methods

    VkPipeline GraphicsPipeline::CreateVulkanGraphicsPipeline() {
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


        auto bindingDescriptions = shader->GetBindingDescription();
        auto attributeDescriptions = shader->GetAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
        vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
        vertexInputStateCreateInfo.pVertexBindingDescriptions = &bindingDescriptions;
        vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        // Draw Triangles
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
        inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        auto swapChainExtent = swapChain->GetExtent();
        viewport.width = (float) swapChainExtent.width;
        viewport.height = (float) swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = {0, 0};
        scissor.extent = swapChainExtent;

        // states that can change without having to recreate the pipeline at draw time
        vector<VkDynamicState> dynamicStates = {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR
        };
        VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
        dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

        VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
        viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportStateCreateInfo.viewportCount = 1;
        viewportStateCreateInfo.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {};
        SetRasterizationStateCreateInfo(rasterizationStateCreateInfo);


        VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
        multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
        multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleStateCreateInfo.minSampleShading = 1.0f; // Optional
        multisampleStateCreateInfo.pSampleMask = nullptr; // Optional
        multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE; // Optional


        VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {};
        colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
                                                   | VK_COLOR_COMPONENT_G_BIT
                                                   | VK_COLOR_COMPONENT_B_BIT
                                                   | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachmentState.blendEnable = VK_FALSE;
        colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD; // Optional
        colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

        VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {};
        colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
        colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlendStateCreateInfo.attachmentCount = 1;
        colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
        colorBlendStateCreateInfo.blendConstants[0] = 0.0f;// Optional
        colorBlendStateCreateInfo.blendConstants[1] = 0.0f;// Optional
        colorBlendStateCreateInfo.blendConstants[2] = 0.0f;// Optional
        colorBlendStateCreateInfo.blendConstants[3] = 0.0f;// Optional

        VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {};
        depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
        depthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
        depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS; // lower depth = closer
        depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
        depthStencilStateCreateInfo.minDepthBounds = 0.0f; // Optional
        depthStencilStateCreateInfo.maxDepthBounds = 1.0f; // Optional
        depthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
        depthStencilStateCreateInfo.front = {}; // Optional
        depthStencilStateCreateInfo.back = {}; // Optional

        VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
        graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        graphicsPipelineCreateInfo.stageCount = 2;
        graphicsPipelineCreateInfo.pStages = shaderStages;
        graphicsPipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
        graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
        graphicsPipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
        graphicsPipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
        graphicsPipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
        graphicsPipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
        graphicsPipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
        graphicsPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
        graphicsPipelineCreateInfo.layout = shader->GetVulkanPipelineLayout();
        graphicsPipelineCreateInfo.renderPass = renderPass->GetVulkanRenderPass();
        graphicsPipelineCreateInfo.subpass = renderPass->GetSubpassIndex();
        graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        graphicsPipelineCreateInfo.basePipelineIndex = -1; // Optional

        VkPipeline graphicsPipeline;
        VkResult createGraphicsPipelineResult = vkCreateGraphicsPipelines(logicalDevice->GetVulkanDevice(),
                                                                          VK_NULL_HANDLE,
                                                                          1,
                                                                          &graphicsPipelineCreateInfo,
                                                                          nullptr,
                                                                          &graphicsPipeline);
        if (createGraphicsPipelineResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Graphics Pipeline!");
        }

        cout << "Successfully Created Graphics Pipeline" << endl;


        return graphicsPipeline;
    }


    // #endregion

    // #region Constructors

    GraphicsPipeline::GraphicsPipeline(PhysicalDevice *physicalDevice,
                                       LogicalDevice *logicalDevice,
                                       SwapChain *swapChain,
                                       uint32_t graphicsFamilyIndex,
                                       bool isFirstToRender,
                                       bool isLastToRender) {
        this->isFirstToRender = isFirstToRender;
        this->isLastToRender = isLastToRender;
        this->physicalDevice = physicalDevice;
        this->logicalDevice = logicalDevice;
        this->swapChain = swapChain;
        drawCommand = new DrawCommand(logicalDevice);
    }

    // #endregion

    // #region Destructors 

    GraphicsPipeline::~GraphicsPipeline() {
        cout << "Destroy Graphics Pipeline" << endl;

        delete renderPass;
        vkDestroyPipeline(logicalDevice->GetVulkanDevice(), vulkanGraphicsPipeline, nullptr);
        delete shader;
        delete drawCommand;
    }

    // #endregion

    // #region Protected Methods

    void GraphicsPipeline::Init() {
        renderPass = CreateRenderPass();

        shader = CreateShader();

        vulkanGraphicsPipeline = CreateVulkanGraphicsPipeline();

        // no longer need shader modules
        shader->CleanUpShaderModules();
    }


    // #endregion

    // #region Public Methods

    VkCommandBuffer GraphicsPipeline::Draw(VkFramebuffer vulkanFrameBuffer, uint32_t currentFrame) {
        drawCommand->Begin(currentFrame);

        auto vulkanCommandBuffer = drawCommand->GetVulkanCommandBuffer(currentFrame);

        auto extent = swapChain->GetExtent();
        renderPass->Begin(vulkanCommandBuffer,
                          vulkanFrameBuffer,
                          extent);

        vkCmdBindPipeline(vulkanCommandBuffer,
                          VK_PIPELINE_BIND_POINT_GRAPHICS,
                          vulkanGraphicsPipeline);

        shader->Bind(vulkanCommandBuffer, currentFrame);

        // Dynamic viewport and scissor
        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(extent.width);
        viewport.height = static_cast<float>(extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(vulkanCommandBuffer, 0, 1, &viewport);

        VkRect2D scissor = {};
        scissor.offset = {0, 0};
        scissor.extent = extent;
        vkCmdSetScissor(vulkanCommandBuffer, 0, 1, &scissor);

        shader->Render(vulkanCommandBuffer, currentFrame, extent);

        renderPass->End(vulkanCommandBuffer);

        drawCommand->End(currentFrame);

        return drawCommand->GetVulkanCommandBuffer(currentFrame);
    }

    // #endregion

} // KTXCompressor