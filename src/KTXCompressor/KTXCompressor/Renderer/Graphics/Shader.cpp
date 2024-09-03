//
// Created by shane on 28/07/2024.
//

#include <fstream>
#include "Shader.h"

namespace KTXCompressor {

    // #region Private Methods

    void Shader::Init() {
        CreateVertexBuffer();
        CreateIndexBuffer();

        depthTexture = new DepthTexture(extent, logicalDevice, physicalDevice);
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
                   VkExtent2D extent,
                   const string &vertexFileName,
                   const string &fragmentFileName) {
        this->physicalDevice = physicalDevice;
        this->logicalDevice = logicalDevice;
        this->extent = extent;
        this->bufferUtil = new BufferUtil(logicalDevice, physicalDevice);

        vertexShaderModule = CreateShaderModule(vertexFileName);
        fragmentShaderModule = CreateShaderModule(fragmentFileName);
    }

    // #endregion

    // #region Destructors 

    Shader::~Shader() {
        cout << "Destroy Shader" << endl;

        delete depthTexture;
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
        VkBuffer vertexBuffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(vulkanCommandBuffer, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(vulkanCommandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

        BindDescriptorSets(vulkanCommandBuffer, currentFrame);
    }


    // #endregion

} // KTXCompressor