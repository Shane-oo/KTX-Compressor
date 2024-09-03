//
// Created by ShaneMonck on 10/08/2024.
//

#include "CopyBufferCommand.h"

namespace KTXCompressor {

    // #region Private Methods

    void CopyBufferCommand::GetPipelineStageFlags(VkImageLayout oldLayout,
                                                  VkImageLayout newLayout,
                                                  VkImageMemoryBarrier &imageMemoryBarrier,
                                                  VkPipelineStageFlags &sourceStage,
                                                  VkPipelineStageFlags &destinationStage) {
        // Undefined -> transfer destination: transfer writes that don't need to wait on anything
        // Transfer Destination -> shader reading: shader reads should wait on transfer writes,
        // specifically the shader reads in the fragment shader, because ttha'ts where were going to use the image

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            imageMemoryBarrier.srcAccessMask = 0;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                   newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else {
            throw invalid_argument("Unsupported Layout Transition");
        }
    }

    VkCommandBuffer CopyBufferCommand::CreatCommandBuffer() {
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
            throw runtime_error("Failed to Allocate CopyBuffers Command Buffer");
        }

        cout << "Successfully Allocated CopyBuffers Command Buffer" << endl;

        return commandBuffer;
    }

    void CopyBufferCommand::Begin() {
        VkCommandBufferBeginInfo commandBufferBeginInfo = {};
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VkResult beginCommandBufferResult = vkBeginCommandBuffer(vulkanCommandBuffer, &commandBufferBeginInfo);
        if (beginCommandBufferResult != VK_SUCCESS) {
            throw runtime_error("Failed to Being CopyBuffers Command Buffer");
        }

        cout << "Successfully Began CopyBuffers Command Buffer" << endl;
    }

    void CopyBufferCommand::CopyBuffers(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        VkBufferCopy bufferCopy = {};
        bufferCopy.srcOffset = 0;
        bufferCopy.dstOffset = 0;
        bufferCopy.size = size;
        vkCmdCopyBuffer(vulkanCommandBuffer, srcBuffer, dstBuffer, 1, &bufferCopy);
    }

    void CopyBufferCommand::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
        VkBufferImageCopy bufferImageCopy = {};
        bufferImageCopy.bufferOffset = 0;
        bufferImageCopy.bufferRowLength = 0;
        bufferImageCopy.bufferImageHeight = 0;

        bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        bufferImageCopy.imageSubresource.mipLevel = 0;
        bufferImageCopy.imageSubresource.baseArrayLayer = 0;
        bufferImageCopy.imageSubresource.layerCount = 1;

        bufferImageCopy.imageOffset = {0, 0, 0};
        bufferImageCopy.imageExtent = {width, height, 1};


        vkCmdCopyBufferToImage(vulkanCommandBuffer,
                               buffer,
                               image,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               1,
                               &bufferImageCopy);
    }

    void CopyBufferCommand::Transition(VkImage image,
                                       VkFormat format,
                                       VkImageLayout oldLayout,
                                       VkImageLayout newLayout) {

        // Barrier
        VkImageMemoryBarrier imageMemoryBarrier{};
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.oldLayout = oldLayout;
        imageMemoryBarrier.newLayout = newLayout;
        imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.image = image;
        imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
        imageMemoryBarrier.subresourceRange.levelCount = 1;
        imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
        imageMemoryBarrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceFlags;
        VkPipelineStageFlags destinationStage;
        GetPipelineStageFlags(oldLayout, newLayout, imageMemoryBarrier, sourceFlags, destinationStage);


        VkDependencyFlags dependencyFlags = 0;
        uint32_t memoryBarrierCount = 0;
        VkMemoryBarrier *pMemoryBarriers = nullptr;
        uint32_t bufferMemoryBarrierCount = 0;
        VkBufferMemoryBarrier *pBufferMemoryBarriers = nullptr;
        uint32_t imageMemoryBarrierCount = 1;
        vkCmdPipelineBarrier(
                vulkanCommandBuffer,
                sourceFlags,
                destinationStage,
                dependencyFlags,
                memoryBarrierCount,
                pMemoryBarriers,
                bufferMemoryBarrierCount,
                pBufferMemoryBarriers,
                imageMemoryBarrierCount,
                &imageMemoryBarrier
        );

        cout << "Successfully Transitioned Image from "
             << to_string(oldLayout)
             << " to "
             << to_string(newLayout)
             << endl;
    }

    void CopyBufferCommand::End() {
        VkResult endCommandBufferResult = vkEndCommandBuffer(vulkanCommandBuffer);
        if (endCommandBufferResult != VK_SUCCESS) {
            throw runtime_error("Failed to End CopyBuffers Command Buffer");
        }

        cout << "Successfully Ended CopyBuffers Command Buffer" << endl;
    }

    void CopyBufferCommand::Submit() {
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &vulkanCommandBuffer;

        logicalDevice->SubmitToGraphicsQueue(submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(logicalDevice->GetGraphicsQueue()->GetVulkanQueue());

        cout << "Successfully Submitted CopyBuffers Buffer Command" << endl;
    }





    // #endregion

    // #region Constructors

    CopyBufferCommand::CopyBufferCommand(LogicalDevice *logicalDevice) : Command(logicalDevice) {
        vulkanCommandBuffer = CreatCommandBuffer();
    }


    // #endregion

    // #region Destructors

    CopyBufferCommand::~CopyBufferCommand() {
        cout << "Destroy CopyBuffers Buffer Command" << endl;

        vkDestroyCommandPool(logicalDevice->GetVulkanDevice(), vulkanCommandPool, nullptr);
    }


    // #endregion

    // #region Public Methods

    void CopyBufferCommand::Copy(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        Begin();
        CopyBuffers(srcBuffer, dstBuffer, size);
        End();
        Submit();
    }

    void CopyBufferCommand::CopyImage(VkImage image,
                                      VkFormat format,
                                      VkBuffer stagingBuffer,
                                      uint32_t width,
                                      uint32_t height) {
        Begin();

        // Transition to Be Ready for Copying
        Transition(image,
                   format,
                   VK_IMAGE_LAYOUT_UNDEFINED,
                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        CopyBufferToImage(stagingBuffer, image, width, height);

        // Transition To Be Ready for Shader Access
        Transition(image,
                   format,
                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        End();
        Submit();
    }







    // #endregion

} // KTXCompressor