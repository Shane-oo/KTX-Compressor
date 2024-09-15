//
// Created by ShaneMonck on 13/08/2024.
//

#ifndef KTX_COMPRESSOR_DESCRIPTORSET_H
#define KTX_COMPRESSOR_DESCRIPTORSET_H

#include <vulkan/vulkan_core.h>
#include "../../../Common.h"
#include "../../Instance/Devices/LogicalDevice.h"
#include "DescriptorPools/DescriptorPool.h"

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
        DescriptorPool* descriptorPool;
        vector<VkDescriptorSet> vulkanDescriptorSets;

        virtual VkDescriptorSetLayoutBinding GetDescriptorSetLayoutBinding() = 0;

        virtual DescriptorPoolSizeModel GetDescriptorPoolSize() = 0;

        virtual void SetWriteDescriptorSet(VkWriteDescriptorSet &writeDescriptorSet,
                                           size_t i) = 0;

        virtual int GetBinding() = 0;

        void Init();

    private:
        VkDescriptorSetLayout CreateDescriptorSetLayout();

        DescriptorPool* CreateDescriptorPool();

        vector<VkDescriptorSet> CreateDescriptorSets();

    public:
        VkDescriptorSetLayout GetVulkanDescriptorSetLayout() {
            return vulkanDescriptorSetLayout;
        }

        DescriptorPool* GetDescriptorPool(){
            return descriptorPool;
        }
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_DESCRIPTORSET_H
