//
// Created by ShaneMonck on 12/09/2024.
//

#ifndef KTX_COMPRESSOR_DESCRIPTORPOOL_H
#define KTX_COMPRESSOR_DESCRIPTORPOOL_H

#include <vulkan/vulkan_core.h>
#include "../../../../Common.h"
#include "../../../Instance/Devices/LogicalDevice.h"

namespace KTXCompressor {

    struct DescriptorPoolSizeModel {
        std::vector<VkDescriptorPoolSize> descriptorPoolSizes;
        VkDescriptorPoolCreateFlags descriptorPoolCreateFlags;
        uint32_t maxSets;

        // Constructor
        DescriptorPoolSizeModel(const std::vector<VkDescriptorPoolSize> &poolSizes,
                                VkDescriptorPoolCreateFlags flags,
                                uint32_t sets)
                : descriptorPoolSizes(poolSizes), descriptorPoolCreateFlags(flags), maxSets(sets) {}
    };

    class DescriptorPool {
    public:
        DescriptorPool(LogicalDevice *logicalDevice, DescriptorPoolSizeModel descriptorPoolSizeModel);

        ~DescriptorPool();

    private:
        LogicalDevice *logicalDevice;
        VkDescriptorPool vulkanDescriptorPool;

        VkDescriptorPool CreateDescriptorPool(DescriptorPoolSizeModel descriptorPoolSizeModel);

    public:
        VkDescriptorPool GetVulkanDescriptorPool() {
            return vulkanDescriptorPool;
        }
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_DESCRIPTORPOOL_H
