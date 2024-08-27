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

        virtual void BindToCommandBuffer(VkCommandBuffer vulkanCommandBuffer,
                                 VkPipelineLayout vulkanPipelineLayout,
                                 uint32_t currentFrame) = 0;

    protected:
        LogicalDevice *logicalDevice;
        VkDescriptorSetLayout vulkanDescriptorSetLayout;
        VkDescriptorPool vulkanDescriptorPool;
        vector<VkDescriptorSet> vulkanDescriptorSets;

        virtual VkDescriptorSetLayoutBinding GetDescriptorSetLayoutBinding() = 0;

        virtual VkDescriptorPoolSize GetDescriptorPoolSize() = 0;

        virtual void SetWriteDescriptorSet(VkWriteDescriptorSet &writeDescriptorSet,
                                           size_t i) = 0;
        

        void Init();

    private:
        VkDescriptorSetLayout CreateDescriptorSetLayout();

        VkDescriptorPool CreateDescriptorPool();
        
        vector<VkDescriptorSet> CreateDescriptorSets();

    public:
        VkDescriptorSetLayout GetVulkanDescriptorSetLayout() {
            return vulkanDescriptorSetLayout;
        }
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_DESCRIPTORSET_H
