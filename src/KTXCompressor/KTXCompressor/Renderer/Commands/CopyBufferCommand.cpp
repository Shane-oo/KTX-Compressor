//
// Created by ShaneMonck on 10/08/2024.
//

#include "CopyBufferCommand.h"

namespace KTXCompressor {

    // #region Private Methods

    VkCommandBuffer CopyBufferCommand::CreateCopyCommandBuffer() {
        VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandPool = vulkanCommandPool;
        commandBufferAllocateInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        VkResult allocateCommandBufferResult = vkAllocateCommandBuffers(logicalDevice->GetVulkanDevice(),
                                                                        &commandBufferAllocateInfo,
                                                                        &commandBuffer);
        if (allocateCommandBufferResult != VK_SUCCESS) {
            throw runtime_error("Failed to Allocate Copy Command Buffer");
        }

        cout << "Successfully Allocated Copy Command Buffer" << endl;

        return commandBuffer;
    }

    // #endregion

    // #region Constructors

    CopyBufferCommand::CopyBufferCommand(LogicalDevice *logicalDevice, VkBuffer srcBuffer, VkBuffer dstBuffer,
                                         VkDeviceSize size) : Command(logicalDevice) {
        vulkanCopyCommandBuffer = CreateCopyCommandBuffer();
        Begin();
        Copy(srcBuffer, dstBuffer, size);
        End();
        Submit();
    }


    // #endregion

    // #region Destructors

    CopyBufferCommand::~CopyBufferCommand() {
        cout << "Destroy Copy Buffer Command" << endl;

        vkDestroyCommandPool(logicalDevice->GetVulkanDevice(), vulkanCommandPool, nullptr);
    }


    // #endregion

    // #region Public Methods

    void CopyBufferCommand::Begin() {
        VkCommandBufferBeginInfo commandBufferBeginInfo = {};
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VkResult beginCommandBufferResult = vkBeginCommandBuffer(vulkanCopyCommandBuffer, &commandBufferBeginInfo);
        if (beginCommandBufferResult != VK_SUCCESS) {
            throw runtime_error("Failed to Being Copy Command Buffer");
        }

        cout << "Successfully Began Copy Command Buffer" << endl;
    }

    void CopyBufferCommand::Copy(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        VkBufferCopy bufferCopy = {};
        bufferCopy.srcOffset = 0;
        bufferCopy.dstOffset = 0;
        bufferCopy.size = size;
        vkCmdCopyBuffer(vulkanCopyCommandBuffer, srcBuffer, dstBuffer, 1, &bufferCopy);
    }

    void CopyBufferCommand::End() {
        VkResult endCommandBufferResult = vkEndCommandBuffer(vulkanCopyCommandBuffer);
        if (endCommandBufferResult != VK_SUCCESS) {
            throw runtime_error("Failed to End Copy Command Buffer");
        }

        cout << "Successfully Ended Copy Command Buffer" << endl;
    }

    void CopyBufferCommand::Submit() {
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &vulkanCopyCommandBuffer;

        logicalDevice->SubmitToGraphicsQueue(submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(logicalDevice->GetGraphicsQueue()->GetVulkanQueue());

        cout << "Successfully Submitted Copy Buffer Command" << endl;
    }

    // #endregion

} // KTXCompressor