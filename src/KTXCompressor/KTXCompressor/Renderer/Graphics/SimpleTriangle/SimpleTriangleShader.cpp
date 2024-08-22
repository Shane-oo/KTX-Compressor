//
// Created by shane on 28/07/2024.
//

#include "SimpleTriangleShader.h"
#include "../Textures/Texture.h"

namespace KTXCompressor {


    // #region Constructors

    SimpleTriangleShader::SimpleTriangleShader(PhysicalDevice *physicalDevice, LogicalDevice *logicalDevice)
            : Shader(physicalDevice, logicalDevice, "simple_triangle.vert.spv", "simple_triangle.frag.spv") {
        Init();

        modelViewProjectionDescriptorSet = new ModelViewProjectionDescriptorSet(logicalDevice, physicalDevice);

        // TODO Image View And sampler: page 202
        woodTexture = new Texture(logicalDevice, physicalDevice, "textures/wood_diffuse_4096x4096.png");

    }

    // #endregion

    // #region Destructors 

    SimpleTriangleShader::~SimpleTriangleShader() {
        cout << "Destroy Simple Triangle Shader" << endl;

        delete modelViewProjectionDescriptorSet;
        delete woodTexture;

        // The base class destructor Shader::~Shader() is automatically called after this
    }

    // #endregion

    // #region Protected Methods

    VkPipelineLayout SimpleTriangleShader::CreatePipelineLayout() {
        VkPipelineLayout pipelineLayout;

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

        auto descriptorSet = modelViewProjectionDescriptorSet->GetVulkanDescriptorSetLayout();
        pipelineLayoutCreateInfo.setLayoutCount = 1;
        pipelineLayoutCreateInfo.pSetLayouts = &descriptorSet;

        pipelineLayoutCreateInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutCreateInfo.pPushConstantRanges = nullptr; // Optional
        pipelineLayoutCreateInfo.pNext = nullptr;
        pipelineLayoutCreateInfo.flags = VK_PIPELINE_LAYOUT_CREATE_INDEPENDENT_SETS_BIT_EXT;

        VkResult pipelineLayoutCreateResult = vkCreatePipelineLayout(logicalDevice->GetVulkanDevice(),
                                                                     &pipelineLayoutCreateInfo,
                                                                     nullptr,
                                                                     &pipelineLayout);

        if (pipelineLayoutCreateResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Pipeline Layout for SimpleTriangleShader");
        }

        cout << "Successfully Created Pipeline Layout for SimpleTriangleShader" << endl;

        return pipelineLayout;
    }

    void SimpleTriangleShader::CreateVertexBuffer() {
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        bufferUtil->CreateAndFillBuffer(vertices.data(),
                                        bufferSize,
                                        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                        vertexBuffer,
                                        vertexBufferMemory);

        cout << "Successfully Created Simple Triangle Vertex Buffer" << endl;
    }

    void SimpleTriangleShader::CreateIndexBuffer() {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        bufferUtil->CreateAndFillBuffer(indices.data(),
                                        bufferSize,
                                        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                        indexBuffer,
                                        indexBufferMemory);

        cout << "Successfully Created Simple Triangle Index Buffer" << endl;
    }

    void SimpleTriangleShader::BindDescriptorSet(VkCommandBuffer vulkanCommandBuffer, uint32_t currentFrame) {
        modelViewProjectionDescriptorSet->BindToCommandBuffer(vulkanCommandBuffer, vulkanPipelineLayout, currentFrame);
    }

    // #endregion

    // #region Public Methods

    const char *SimpleTriangleShader::GetVertexEntryPointName() {
        return "main";
    }

    const char *SimpleTriangleShader::GetFragmentEntryPointName() {
        return "main";
    }

    void SimpleTriangleShader::Render(VkCommandBuffer vulkanCommandBuffer, uint32_t currentFrame, VkExtent2D extent) {
        modelViewProjectionDescriptorSet->Update(currentFrame, extent);
        vkCmdDrawIndexed(vulkanCommandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
    }





    // #endregion

} // KTXCompressor