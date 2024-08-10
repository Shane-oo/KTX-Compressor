//
// Created by ShaneMonck on 10/08/2024.
//

#ifndef KTX_COMPRESSOR_COMMAND_H
#define KTX_COMPRESSOR_COMMAND_H

#include <vulkan/vulkan_core.h>
#include "../../Common.h"

namespace KTXCompressor {

    class Command {
    public:
        Command(VkDevice vulkanDevice, uint32_t graphicsFamilyIndex);
    protected:
        VkDevice vulkanDevice;
        VkCommandPool vulkanCommandPool;

        VkCommandPool CreateVulkanCommandPool(uint32_t graphicsFamilyIndex);

    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_COMMAND_H
