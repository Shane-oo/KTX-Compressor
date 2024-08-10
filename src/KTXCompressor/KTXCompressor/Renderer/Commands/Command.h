//
// Created by ShaneMonck on 10/08/2024.
//

#ifndef KTX_COMPRESSOR_COMMAND_H
#define KTX_COMPRESSOR_COMMAND_H

#include <vulkan/vulkan_core.h>
#include "../../Common.h"
#include "../Instance/Devices/LogicalDevice.h"

namespace KTXCompressor {

    class Command {
    public:
        Command(LogicalDevice *logicalDevice);

    protected:
        LogicalDevice* logicalDevice;
        VkCommandPool vulkanCommandPool;

        VkCommandPool CreateVulkanCommandPool();

    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_COMMAND_H
