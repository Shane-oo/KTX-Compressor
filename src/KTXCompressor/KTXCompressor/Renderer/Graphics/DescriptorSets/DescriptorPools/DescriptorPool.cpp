//
// Created by ShaneMonck on 12/09/2024.
//

#include "DescriptorPool.h"

#include <utility>

namespace KTXCompressor {

    // #region Private Methods

    VkDescriptorPool DescriptorPool::CreateDescriptorPool(DescriptorPoolSizeModel descriptorPoolSizeModel) {
        VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
        descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolCreateInfo.poolSizeCount = descriptorPoolSizeModel.descriptorPoolSizes.size();
        descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSizeModel.descriptorPoolSizes.data();
        descriptorPoolCreateInfo.maxSets = descriptorPoolSizeModel.maxSets;
        descriptorPoolCreateInfo.flags = descriptorPoolSizeModel.descriptorPoolCreateFlags;

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

    // #endregion

    // #region Constructors
    DescriptorPool::DescriptorPool(LogicalDevice *logicalDevice,
                                   DescriptorPoolSizeModel descriptorPoolSizeModel) {
        this->logicalDevice = logicalDevice;
        vulkanDescriptorPool = CreateDescriptorPool(std::move(descriptorPoolSizeModel));
    }

    // #endregion

    // #region Destructors

    DescriptorPool::~DescriptorPool() {
        vkDestroyDescriptorPool(logicalDevice->GetVulkanDevice(), vulkanDescriptorPool, nullptr);
    }
    // #endregion

} // KTXCompressor