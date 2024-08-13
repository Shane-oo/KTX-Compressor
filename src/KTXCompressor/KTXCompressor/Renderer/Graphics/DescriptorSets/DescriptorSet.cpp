//
// Created by ShaneMonck on 13/08/2024.
//

#include "DescriptorSet.h"

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
        vulkanDescriptorSetLayout = CreateDescriptorSetLayout();
        vulkanDescriptorPool = CreateDescriptorPool();
        vulkanDescriptorSets = CreateDescriptorSets();
    }


    // #endregion

    // #region Public Methods

    void DescriptorSet::BindToCommandBuffer(VkCommandBuffer vulkanCommandBuffer,
                                            VkPipelineLayout vulkanPipelineLayout,
                                            uint32_t currentFrame) {
        // might be diff for other Descriptor Sets...
        vkCmdBindDescriptorSets(vulkanCommandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                vulkanPipelineLayout,
                                0,
                                1,
                                &vulkanDescriptorSets[currentFrame], 0, nullptr);
    }
    // #endregion

} // KTXCompressor