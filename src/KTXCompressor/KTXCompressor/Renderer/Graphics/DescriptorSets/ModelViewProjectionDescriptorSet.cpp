//
// Created by ShaneMonck on 13/08/2024.
//



#include "ModelViewProjectionDescriptorSet.h"
#include "../../RendererConstants.h"

namespace KTXCompressor {

    // #region Private Methods

    void ModelViewProjectionDescriptorSet::CreateModelViewProjectionUniformBuffers() {
        VkDeviceSize bufferSize = sizeof(ModelViewProjectionUbo);

        vulkanUniformBuffers.resize(RendererConstants::MAX_FRAMES_IN_FLIGHT);
        vulkanUniformBuffersMemory.resize(RendererConstants::MAX_FRAMES_IN_FLIGHT);
        vulkanUniformBuffersMapped.resize(RendererConstants::MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < RendererConstants::MAX_FRAMES_IN_FLIGHT; i++) {
            bufferUtil->CreateBuffer(bufferSize,
                                     VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                     vulkanUniformBuffers[i],
                                     vulkanUniformBuffersMemory[i]);

            vkMapMemory(logicalDevice->GetVulkanDevice(),
                        vulkanUniformBuffersMemory[i],
                        0,
                        bufferSize,
                        0,
                        &vulkanUniformBuffersMapped[i]);
        }
    }


    // #endregion

    // #region Constructors

    ModelViewProjectionDescriptorSet::ModelViewProjectionDescriptorSet(LogicalDevice *logicalDevice,
                                                                       PhysicalDevice *physicalDevice)
            : DescriptorSet(logicalDevice) {
        this->bufferUtil = new BufferUtil(logicalDevice, physicalDevice);
        CreateModelViewProjectionUniformBuffers();

        Init();
    }

    // #endregion

    // #region Destructors

    ModelViewProjectionDescriptorSet::~ModelViewProjectionDescriptorSet() {
        for (size_t i = 0; i < RendererConstants::MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(logicalDevice->GetVulkanDevice(), vulkanUniformBuffers[i], nullptr);
            vkFreeMemory(logicalDevice->GetVulkanDevice(), vulkanUniformBuffersMemory[i], nullptr);
        }
    }

    // #endregion

    // #region Protected Methods


    VkDescriptorSetLayoutBinding ModelViewProjectionDescriptorSet::GetDescriptorSetLayoutBinding() {
        VkDescriptorSetLayoutBinding modelViewProjectionUboLayoutBinding = {};
        modelViewProjectionUboLayoutBinding.binding = GetBinding();
        modelViewProjectionUboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        modelViewProjectionUboLayoutBinding.descriptorCount = 1;// not an array
        modelViewProjectionUboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        modelViewProjectionUboLayoutBinding.pImmutableSamplers = nullptr; // Optional


        return modelViewProjectionUboLayoutBinding;
    }

    VkDescriptorPoolSize ModelViewProjectionDescriptorSet::GetDescriptorPoolSize() {
        VkDescriptorPoolSize descriptorPoolSize = {};
        descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorPoolSize.descriptorCount = static_cast<uint32_t>(RendererConstants::MAX_FRAMES_IN_FLIGHT);

        return descriptorPoolSize;
    }

    void ModelViewProjectionDescriptorSet::SetWriteDescriptorSet(VkWriteDescriptorSet &writeDescriptorSet,
                                                                 size_t i) {
        if (descriptorBufferInfos.size() <= i) {
            descriptorBufferInfos.resize(i + 1);
        }
        descriptorBufferInfos[i].buffer = vulkanUniformBuffers[i];
        descriptorBufferInfos[i].offset = 0;
        descriptorBufferInfos[i].range = sizeof(ModelViewProjectionUbo);

        writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.dstBinding = GetBinding();
        writeDescriptorSet.dstArrayElement = 0;
        writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptorSet.descriptorCount = 1;
        writeDescriptorSet.pBufferInfo = &descriptorBufferInfos[i];
        writeDescriptorSet.pImageInfo = nullptr; // Optional
        writeDescriptorSet.pTexelBufferView = nullptr;//Optional
    }

    // #endregion

    // #region Public Methods

    void ModelViewProjectionDescriptorSet::Update(uint32_t currentFrame, VkExtent2D extent) {
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();

        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        ModelViewProjectionUbo mvpUbo = {};
        // rotate around z axis
        mvpUbo.model = glm::rotate(glm::mat4(1.0f), deltaTime * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        // look at geometry from above at 45 degree angle
        mvpUbo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
                                  glm::vec3(0.0f, 0.0f, 0.0f),
                                  glm::vec3(0.0f, 0.0f, 1.0f));

        mvpUbo.proj = glm::perspective(glm::radians(45.0f), (float) extent.width / (float) extent.height, 0.1f, 10.0f);

        // Vulkan not OpenGL => flip Y
        mvpUbo.proj[1][1] *= -1;

        memcpy(vulkanUniformBuffersMapped[currentFrame], &mvpUbo, sizeof(mvpUbo));
    }

    // #endregion

} // KTXCompressor