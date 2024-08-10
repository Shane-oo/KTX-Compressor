//
// Created by ShaneMonck on 10/08/2024.
//

#include "Command.h"

namespace KTXCompressor {


    // #region Constructors

    Command::Command(VkDevice vulkanDevice, uint32_t graphicsFamilyIndex) {
        this->vulkanDevice = vulkanDevice;
        vulkanCommandPool = CreateVulkanCommandPool(graphicsFamilyIndex);
    }

    // #endregion

    // #region Protected Methods

    VkCommandPool Command::CreateVulkanCommandPool(uint32_t graphicsFamilyIndex) {
        VkCommandPoolCreateInfo commandPoolCreateInfo = {};
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        // allow command buffers to be recorded individually
        commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        commandPoolCreateInfo.queueFamilyIndex = graphicsFamilyIndex;

        VkCommandPool commandPool;
        VkResult createCommandPoolResult = vkCreateCommandPool(vulkanDevice,
                                                               &commandPoolCreateInfo,
                                                               nullptr,
                                                               &commandPool);

        if (createCommandPoolResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Draw Command Pool");
        }

        cout << "Successfully Created Command Pool" << endl;

        return commandPool;
    }

    // #endregion
} // KTXCompressor