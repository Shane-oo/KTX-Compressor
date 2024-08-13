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


    VkDescriptorSetLayout ModelViewProjectionDescriptorSet::CreateDescriptorSetLayout() {
        VkDescriptorSetLayoutBinding modelViewProjectionUboLayoutBinding = {};
        modelViewProjectionUboLayoutBinding.binding = 0;
        modelViewProjectionUboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        modelViewProjectionUboLayoutBinding.descriptorCount = 1;// not an array
        modelViewProjectionUboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        modelViewProjectionUboLayoutBinding.pImmutableSamplers = nullptr; // Optional

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
        descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutCreateInfo.bindingCount = 1;
        descriptorSetLayoutCreateInfo.pBindings = &modelViewProjectionUboLayoutBinding;

        VkDescriptorSetLayout descriptorSetLayout;
        VkResult createDescriptorSetLayoutResult = vkCreateDescriptorSetLayout(logicalDevice->GetVulkanDevice(),
                                                                               &descriptorSetLayoutCreateInfo,
                                                                               nullptr,
                                                                               &descriptorSetLayout);
        if (createDescriptorSetLayoutResult != VK_SUCCESS) {
            throw runtime_error("Failed To Create ModelViewProjectionDescriptorSet");
        }

        cout << "Successfully Created ModelViewProjectionDescriptorSet" << endl;

        return descriptorSetLayout;
    }

    VkDescriptorPool ModelViewProjectionDescriptorSet::CreateDescriptorPool() {
        VkDescriptorPoolSize descriptorPoolSize = {};
        descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorPoolSize.descriptorCount = static_cast<uint32_t>(RendererConstants::MAX_FRAMES_IN_FLIGHT);

        VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
        descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolCreateInfo.poolSizeCount = 1;
        descriptorPoolCreateInfo.pPoolSizes = &descriptorPoolSize;
        descriptorPoolCreateInfo.maxSets = static_cast<uint32_t>(RendererConstants::MAX_FRAMES_IN_FLIGHT);

        VkDescriptorPool descriptorPool;
        VkResult createDescriptorPoolResult = vkCreateDescriptorPool(logicalDevice->GetVulkanDevice(),
                                                                     &descriptorPoolCreateInfo,
                                                                     nullptr,
                                                                     &descriptorPool);
        if (createDescriptorPoolResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create ModelViewProjection Descriptor Pool");
        }

        cout << "Successfully Created ModelViewProjection Descriptor Pool" << endl;

        return descriptorPool;
    }

    // could this be in the parent?
    vector<VkDescriptorSet> ModelViewProjectionDescriptorSet::CreateDescriptorSets() {
        //In our case we will create one descriptor set for each frame in flight, all with the same layout

        vector<VkDescriptorSetLayout> descriptorSetLayouts(RendererConstants::MAX_FRAMES_IN_FLIGHT,
                                                           vulkanDescriptorSetLayout);

        VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
        descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptorSetAllocateInfo.descriptorPool = vulkanDescriptorPool;
        descriptorSetAllocateInfo.descriptorSetCount = static_cast<uint32_t>(RendererConstants::MAX_FRAMES_IN_FLIGHT);
        descriptorSetAllocateInfo.pSetLayouts = descriptorSetLayouts.data();

        vector<VkDescriptorSet> descriptorSets(RendererConstants::MAX_FRAMES_IN_FLIGHT);
        VkResult allocateDescriptorSetsResult = vkAllocateDescriptorSets(logicalDevice->GetVulkanDevice(),
                                                                         &descriptorSetAllocateInfo,
                                                                         descriptorSets.data());
        if (allocateDescriptorSetsResult != VK_SUCCESS) {
            throw runtime_error("Failed to Allocate Descriptor Sets!");
        }

        cout << "Successfully Allocated Descriptor Sets" << endl;

        /* Descriptors that refer to buffers, like our uniform buffer descriptor, are configured
         * with a VkDescriptorBufferInfo struct. This structure specifies the buffer 
         * and the region within it that contains the data for the descriptor
        */
        for (size_t i = 0; i < RendererConstants::MAX_FRAMES_IN_FLIGHT; i++) {
            VkDescriptorBufferInfo descriptorBufferInfo = {};
            descriptorBufferInfo.buffer = vulkanUniformBuffers[i];
            descriptorBufferInfo.offset = 0;
            descriptorBufferInfo.range = sizeof(ModelViewProjectionUbo);

            VkWriteDescriptorSet writeDescriptorSet = {};
            writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptorSet.dstSet = descriptorSets[i];
            writeDescriptorSet.dstBinding = 0;
            writeDescriptorSet.dstArrayElement = 0;
            writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeDescriptorSet.descriptorCount = 1;
            writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;
            writeDescriptorSet.pImageInfo = nullptr; // Optional
            writeDescriptorSet.pTexelBufferView = nullptr;//Optional

            vkUpdateDescriptorSets(logicalDevice->GetVulkanDevice(), 1, &writeDescriptorSet, 0, nullptr);
        }

        return descriptorSets;
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