//
// Created by ShaneMonck on 13/08/2024.
//

#include "DescriptorSet.h"
#include "../../RendererConstants.h"

namespace KTXCompressor {

    // #region Constructors

    DescriptorSet::DescriptorSet(LogicalDevice *logicalDevice) {
        this->logicalDevice = logicalDevice;
    }

    // #endregion

    // #region Destructors

    DescriptorSet::~DescriptorSet() {
        cout << "Destroy Descriptor Set" << endl;

        vkDestroyDescriptorPool(logicalDevice->GetVulkanDevice(), vulkanDescriptorPool, nullptr);

        vkDestroyDescriptorSetLayout(logicalDevice->GetVulkanDevice(), vulkanDescriptorSetLayout, nullptr);
    }

    // #endregion

    // #region Protected Methods

    void DescriptorSet::Init() {
        // "Rule of thumb is probably "less is more". If you do not need the resources to have separate types or names, use array.
        // If you do not need separate set, use only one set."
        // Unfortunately the way this currently set up is I cant have just one set.

        vulkanDescriptorSetLayout = CreateDescriptorSetLayout();
        vulkanDescriptorPool = CreateDescriptorPool();
        vulkanDescriptorSets = CreateDescriptorSets();
    }


    // #endregion

    // #region Public Methods


    VkDescriptorSetLayout DescriptorSet::CreateDescriptorSetLayout() {
        // Get Binding from child
        VkDescriptorSetLayoutBinding binding = GetDescriptorSetLayoutBinding();

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
        descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutCreateInfo.bindingCount = 1;
        descriptorSetLayoutCreateInfo.pBindings = &binding;

        VkDescriptorSetLayout descriptorSetLayout;
        VkResult createDescriptorSetLayoutResult = vkCreateDescriptorSetLayout(logicalDevice->GetVulkanDevice(),
                                                                               &descriptorSetLayoutCreateInfo,
                                                                               nullptr,
                                                                               &descriptorSetLayout);
        if (createDescriptorSetLayoutResult != VK_SUCCESS) {
            throw runtime_error("Failed To Create descriptorSetLayout");
        }

        cout << "Successfully Created descriptorSetLayout" << endl;

        return descriptorSetLayout;
    }

    VkDescriptorPool DescriptorSet::CreateDescriptorPool() {
        // Get descriptorPoolSize from child
        VkDescriptorPoolSize descriptorPoolSize = GetDescriptorPoolSize();

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
            throw runtime_error("Failed to Create Descriptor Pool");
        }

        cout << "Successfully Created Descriptor Pool" << endl;

        return descriptorPool;
    }

    vector<VkDescriptorSet> DescriptorSet::CreateDescriptorSets() {
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
            VkWriteDescriptorSet writeDescriptorSet = {};
            writeDescriptorSet.dstSet = descriptorSets[i];
            // Get Child to Set WriteDescriptorSett
            SetWriteDescriptorSet(writeDescriptorSet, i);

            vkUpdateDescriptorSets(logicalDevice->GetVulkanDevice(), 1, &writeDescriptorSet, 0, nullptr);
        }

        return descriptorSets;
    }

    // #endregion

} // KTXCompressor