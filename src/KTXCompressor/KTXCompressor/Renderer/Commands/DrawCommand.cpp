//
// Created by shane on 6/08/2024.
//

#include "DrawCommand.h"
#include "../Graphics/GraphicsPipeline.h"

namespace KTXCompressor {

    // #region Private Methods

    VkCommandPool DrawCommand::CreateVulkanCommandPool(uint32_t graphicsFamilyIndex) {
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

    VkCommandBuffer DrawCommand::CreateVulkanCommandBuffer() {
        VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.commandPool = vulkanCommandPool;
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        VkResult allocateCommandBuffersResult = vkAllocateCommandBuffers(vulkanDevice,
                                                                         &commandBufferAllocateInfo,
                                                                         &commandBuffer);

        if (allocateCommandBuffersResult != VK_SUCCESS) {
            throw runtime_error("Failed to Allocate Command Buffers For Draw Command");
        }

        cout << "Successfully Allocated Draw Command Buffer" << endl;

        return commandBuffer;
    }

    // #endregion

    // #region Constructors

    DrawCommand::DrawCommand(VkDevice vulkanDevice, uint32_t graphicsFamilyIndex) {
        this->vulkanDevice = vulkanDevice;
        vulkanCommandPool = CreateVulkanCommandPool(graphicsFamilyIndex);
        vulkanCommandBuffer = CreateVulkanCommandBuffer();
    }

    // #endregion

    // #region Destructors

    DrawCommand::~DrawCommand() {
        cout << "Destroy Draw Command" << endl;

        vkDestroyCommandPool(vulkanDevice, vulkanCommandPool, nullptr);
    }

    // #endregion

    // #region Public Methods

    void DrawCommand::Begin() {
        // reset to make sure it can be recorded to
        vkResetCommandBuffer(vulkanCommandBuffer, 0);

        VkCommandBufferBeginInfo commandBufferBeginInfo = {};
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags = 0; // Optional
        commandBufferBeginInfo.pInheritanceInfo = nullptr; // Optional

        VkResult beginCommandBufferResult = vkBeginCommandBuffer(vulkanCommandBuffer, &commandBufferBeginInfo);

        if (beginCommandBufferResult != VK_SUCCESS) {
            throw runtime_error("Failed to Begin Recording Draw Command Buffer");
        }
    }

    void DrawCommand::End() {
        VkResult endCommandBufferResult = vkEndCommandBuffer(vulkanCommandBuffer);

        if (endCommandBufferResult != VK_SUCCESS) {
            throw runtime_error("Failed To End Command Buffer!");
        }

        //cout << "Successfully Ended Command Buffer" << endl;
    }

    // #endregion

} // KTXCompressor