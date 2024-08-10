//
// Created by shane on 28/07/2024.
//

#include "SimpleTriangleShader.h"

namespace KTXCompressor {


    // #region Constructors

    SimpleTriangleShader::SimpleTriangleShader(PhysicalDevice *physicalDevice, VkDevice vulkanDevice)
            : Shader(physicalDevice, vulkanDevice, "simple_triangle.vert.spv", "simple_triangle.frag.spv") {
        Init();
    }

    // #endregion

    // #region Protected Methods

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

    VkBuffer SimpleTriangleShader::CreateVertexBuffer() {
        VkBufferCreateInfo createBufferInfo = {};
        createBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        createBufferInfo.size = sizeof(vertices[0]) * vertices.size();
        createBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        createBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VkBuffer buffer;
        VkResult createBufferResult = vkCreateBuffer(vulkanDevice, &createBufferInfo, nullptr, &buffer);
        if (createBufferResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Vertex Buffer");
        }

        cout << "Successfully Created Vertex Buffer" << endl;

        return buffer;
    }

    void SimpleTriangleShader::FillVertexBuffer() {
        void *data;
        size_t size = sizeof(vertices[0]) * vertices.size();
        vkMapMemory(vulkanDevice, vertexBufferMemory, 0, size, 0, &data);
        memcpy(data, vertices.data(), size);
        vkUnmapMemory(vulkanDevice, vertexBufferMemory);

        cout << "Vertex Buffer Filled" << endl;
    }

    // #endregion

    // #region Public Methods

    const char *SimpleTriangleShader::GetVertexEntryPointName() {
        return "main";
    }

    const char *SimpleTriangleShader::GetFragmentEntryPointName() {
        return "main";
    }

    void SimpleTriangleShader::Render(VkCommandBuffer vulkanCommandBuffer) {
        vkCmdDraw(vulkanCommandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);
    }

    // #endregion

} // KTXCompressor