//
// Created by ShaneMonck on 13/08/2024.
//

#ifndef KTX_COMPRESSOR_DESCRIPTORSET_H
#define KTX_COMPRESSOR_DESCRIPTORSET_H

#include <vulkan/vulkan_core.h>
#include "../../../Common.h"
#include "../../Instance/Devices/LogicalDevice.h"

namespace KTXCompressor {

    class DescriptorSet {
    public:
        DescriptorSet(LogicalDevice *logicalDevice);

        ~DescriptorSet();

        void BindToCommandBuffer(VkCommandBuffer vulkanCommandBuffer,
                                 VkPipelineLayout vulkanPipelineLayout,
                                 uint32_t currentFrame);

    protected:
        LogicalDevice *logicalDevice;
        VkDescriptorSetLayout vulkanDescriptorSetLayout;
        VkDescriptorPool vulkanDescriptorPool;
        vector<VkDescriptorSet> vulkanDescriptorSets;

        virtual VkDescriptorSetLayout CreateDescriptorSetLayout() = 0;

        virtual VkDescriptorPool CreateDescriptorPool() = 0;

        virtual vector<VkDescriptorSet> CreateDescriptorSets() = 0;

        void Init();

    public:
        VkDescriptorSetLayout GetVulkanDescriptorSetLayout() {
            return vulkanDescriptorSetLayout;
        }
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_DESCRIPTORSET_H
