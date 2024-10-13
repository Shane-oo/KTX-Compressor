//
// Created by shane on 28/07/2024.
//

#include <random>
#include "SimpleTriangleShader.h"
#include "../DescriptorSets/CombinedImageSamplerDescriptorSet.h"

namespace KTXCompressor {


    // #region Constructors

    SimpleTriangleShader::SimpleTriangleShader(PhysicalDevice *physicalDevice,
                                               LogicalDevice *logicalDevice,
                                               RenderPass *renderPass,
                                               VkExtent2D swapChainExtent,
                                               string imagePath)
            : Shader(physicalDevice,
                     logicalDevice,
                     renderPass,
                     swapChainExtent,
                     "simple_triangle.vert.spv",
                     "simple_triangle.frag.spv") {
        modelViewProjectionDescriptorSet = new ModelViewProjectionDescriptorSet(logicalDevice, physicalDevice);
        descriptorSets.push_back(modelViewProjectionDescriptorSet);

        Texture *texture;

        if (imagePath.substr(imagePath.size() - 5) == ".ktx2") {
            texture = new KTXTexture(imagePath, logicalDevice,
                                     physicalDevice); // Use imagePath instead of hardcoded path
        } else {
            texture = new ImageTexture(imagePath, logicalDevice, physicalDevice);
        }

        combinedImageSamplerDescriptorSet = new CombinedImageSamplerDescriptorSet(logicalDevice, texture);

        descriptorSets.push_back(combinedImageSamplerDescriptorSet);

        Init(renderPass, swapChainExtent);
    }

    // #endregion

    // #region Destructors 

    SimpleTriangleShader::~SimpleTriangleShader() {
        cout << "Destroy Simple Triangle Shader" << endl;

        delete modelViewProjectionDescriptorSet;
        delete combinedImageSamplerDescriptorSet;

        // The base class destructor Shader::~Shader() is automatically called after this
    }

    // #endregion

    // #region Protected Methods

    VkPipelineLayout SimpleTriangleShader::CreatePipelineLayout() {
        VkPipelineLayout pipelineLayout;

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

        vector<VkDescriptorSetLayout> descriptorSets{2};
        descriptorSets[0] = modelViewProjectionDescriptorSet->GetVulkanDescriptorSetLayout();
        descriptorSets[1] = combinedImageSamplerDescriptorSet->GetVulkanDescriptorSetLayout();

        pipelineLayoutCreateInfo.setLayoutCount = descriptorSets.size();
        pipelineLayoutCreateInfo.pSetLayouts = descriptorSets.data();

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
        // Create a random device and initialize a random number generator
        std::random_device rd;     // Non-deterministic seed
        std::mt19937 gen(rd());    // Mersenne Twister random number generator

        // Define a uniform distribution for floating-point values between 0 and 1
        std::uniform_real_distribution<float> distrib(0.0f, 1.0f);

        float randomDisplacement = distrib(gen);
        // Loop through vertices and randomly displace positions
        for (auto &vertex: vertices) {
            // Randomly displace x and z coordinates
            vertex.pos.x += randomDisplacement;
            vertex.pos.z += randomDisplacement;
        }

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

    void SimpleTriangleShader::SetRasterizationStateCreateInfo(
            VkPipelineRasterizationStateCreateInfo &rasterizationStateCreateInfo) {
        // cant see this changing in different shader pipelines...
        rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
        rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizationStateCreateInfo.lineWidth = 1.0f;
        rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
        rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f; // Optional
        rasterizationStateCreateInfo.depthBiasClamp = 0.0f; // Optional
        rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f; // Optional
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