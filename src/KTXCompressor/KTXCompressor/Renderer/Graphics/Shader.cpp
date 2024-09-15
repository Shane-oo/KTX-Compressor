//
// Created by shane on 28/07/2024.
//

#include <fstream>
#include "Shader.h"

namespace KTXCompressor {

    // #region Private Methods

    void Shader::Init(RenderPass *renderPass, VkExtent2D swapChainExtent) {
        shaderPipeline = CreateVulkanPipeline(renderPass, swapChainExtent);

        CreateVertexBuffer();
        CreateIndexBuffer();

        CleanUpShaderModules();
    }

    vector<char> Shader::ReadFile(const string &fileName) {
        ifstream file(fileName, ios::ate | ios::binary);

        if (!file.is_open()) {
            throw runtime_error("Failed to Open File: " + fileName);
        }

        streamsize fileSize = file.tellg();
        vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    VkShaderModule Shader::CreateShaderModule(const string &fileName) {
        auto code = ReadFile(fileName);

        VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
        shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleCreateInfo.codeSize = code.size();
        shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

        VkShaderModule shaderModule;
        VkResult createShaderModuleResult = vkCreateShaderModule(logicalDevice->GetVulkanDevice(),
                                                                 &shaderModuleCreateInfo,
                                                                 nullptr,
                                                                 &shaderModule);
        if (createShaderModuleResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Shader Module for " + fileName);
        }

        cout << "Successfully Created Shader Module for " << fileName << endl;

        return shaderModule;
    }

    VkPipeline Shader::CreateVulkanPipeline(RenderPass *renderPass, VkExtent2D swapChainExtent) {
        VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {};
        vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertexShaderStageInfo.module = GetVertexShaderModule();
        vertexShaderStageInfo.pName = GetVertexEntryPointName();

        VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {};
        fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragmentShaderStageInfo.module = GetFragmentShaderModule();
        fragmentShaderStageInfo.pName = GetFragmentEntryPointName();

        VkPipelineShaderStageCreateInfo shaderStages[]{
                vertexShaderStageInfo,
                fragmentShaderStageInfo
        };

        auto bindingDescriptions = GetBindingDescription();
        auto attributeDescriptions = GetAttributeDescriptions();

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

        VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
        pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineCreateInfo.stageCount = 2;
        pipelineCreateInfo.pStages = shaderStages;
        pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
        pipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
        pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
        pipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
        pipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
        pipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
        pipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
        pipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
        pipelineCreateInfo.layout = GetVulkanPipelineLayout();
        pipelineCreateInfo.renderPass = renderPass->GetVulkanRenderPass();
        pipelineCreateInfo.subpass = renderPass->GetSubpassIndex();
        pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineCreateInfo.basePipelineIndex = -1; // Optional

        VkPipeline pipeline;
        VkResult createPipelineResult = vkCreateGraphicsPipelines(logicalDevice->GetVulkanDevice(),
                                                                  VK_NULL_HANDLE,
                                                                  1,
                                                                  &pipelineCreateInfo,
                                                                  nullptr,
                                                                  &pipeline);
        if (createPipelineResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Shader Pipeline!");
        }

        cout << "Successfully Created Shader Pipeline" << endl;


        return pipeline;
    }

    void Shader::BindDescriptorSets(VkCommandBuffer vulkanCommandBuffer, uint32_t currentFrame) {
        for (auto descriptorSet: descriptorSets) {
            descriptorSet->BindToCommandBuffer(vulkanCommandBuffer,
                                               vulkanPipelineLayout,
                                               currentFrame);
        }
    }

    // #endregion


    // #region Constructors

    Shader::Shader(PhysicalDevice *physicalDevice,
                   LogicalDevice *logicalDevice,
                   RenderPass *renderPass,
                   VkExtent2D swapChainExtent,
                   const string &vertexFileName,
                   const string &fragmentFileName) {
        this->physicalDevice = physicalDevice;
        this->logicalDevice = logicalDevice;
        this->bufferUtil = new BufferUtil(logicalDevice, physicalDevice);

        vertexShaderModule = CreateShaderModule(vertexFileName);
        fragmentShaderModule = CreateShaderModule(fragmentFileName);
    }

    // #endregion

    // #region Destructors 

    Shader::~Shader() {
        cout << "Destroy Shader" << endl;

        vkDestroyPipeline(logicalDevice->GetVulkanDevice(), shaderPipeline, nullptr);

        vkDestroyBuffer(logicalDevice->GetVulkanDevice(), vertexBuffer, nullptr);
        vkFreeMemory(logicalDevice->GetVulkanDevice(), vertexBufferMemory, nullptr);
        vkDestroyBuffer(logicalDevice->GetVulkanDevice(), indexBuffer, nullptr);
        vkFreeMemory(logicalDevice->GetVulkanDevice(), indexBufferMemory, nullptr);
        vkDestroyPipelineLayout(logicalDevice->GetVulkanDevice(), vulkanPipelineLayout, nullptr);
    }


    // #endregion

    // #region Protected Methods



    // #endregion

    // #region Public Methods

    VkPipelineLayout Shader::GetVulkanPipelineLayout() {
        if (!vulkanPipelineLayout) {
            vulkanPipelineLayout = CreatePipelineLayout();
        }
        return vulkanPipelineLayout;
    }


    VkVertexInputBindingDescription Shader::GetBindingDescription() {
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    vector<VkVertexInputAttributeDescription> Shader::GetAttributeDescriptions() {
        vector<VkVertexInputAttributeDescription> attributeDescriptions{3};
        // Position
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        // Colour
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, colour);

        // Texture
        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    void Shader::CleanUpShaderModules() {
        cout << "Destroy Shader Modules" << endl;
        vkDestroyShaderModule(logicalDevice->GetVulkanDevice(), fragmentShaderModule, nullptr);
        vkDestroyShaderModule(logicalDevice->GetVulkanDevice(), vertexShaderModule, nullptr);

        delete bufferUtil; // no longer needed
    }

    void Shader::Bind(VkCommandBuffer vulkanCommandBuffer, uint32_t currentFrame) {
        vkCmdBindPipeline(vulkanCommandBuffer,
                          VK_PIPELINE_BIND_POINT_GRAPHICS,
                          shaderPipeline);

        VkBuffer vertexBuffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(vulkanCommandBuffer, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(vulkanCommandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

        BindDescriptorSets(vulkanCommandBuffer, currentFrame);
    }


    // #endregion

} // KTXCompressor