//
// Created by shane on 28/07/2024.
//

#include "SimpleTriangleShader.h"
#include "../DescriptorSets/CombinedImageSamplerDescriptorSet.h"

namespace KTXCompressor {


    // #region Constructors

    SimpleTriangleShader::SimpleTriangleShader(PhysicalDevice *physicalDevice,
                                               LogicalDevice *logicalDevice,
                                               uint32_t speed)
            : Shader(physicalDevice, logicalDevice, "simple_triangle.vert.spv", "simple_triangle.frag.spv") {
        this->speed = speed;
        Init();

        modelViewProjectionDescriptorSet = new ModelViewProjectionDescriptorSet(logicalDevice, physicalDevice);
        descriptorSets.push_back(modelViewProjectionDescriptorSet);

        ktxTexture = new KTXTexture("textures/SAMPLE_2d_rgba8.ktx2", logicalDevice, physicalDevice);
        imageTexture = new ImageTexture("textures/wood_diffuse_4096x4096.png", logicalDevice, physicalDevice);

        auto texture = speed == 1 ? reinterpret_cast<ImageTexture *>(ktxTexture) : imageTexture;
        combinedImageSamplerDescriptorSet = new CombinedImageSamplerDescriptorSet(logicalDevice, texture);
        descriptorSets.push_back(combinedImageSamplerDescriptorSet);
    }

    // #endregion

    // #region Destructors 

    SimpleTriangleShader::~SimpleTriangleShader() {
        cout << "Destroy Simple Triangle Shader" << endl;

        delete modelViewProjectionDescriptorSet;
        delete combinedImageSamplerDescriptorSet;
        delete ktxTexture;
        delete imageTexture;

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
        if(speed == 1){
            for (auto& vertex : vertices) {
                vertex.pos.x += 0.25f;
                vertex.pos.z += 0.25f;
            }
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