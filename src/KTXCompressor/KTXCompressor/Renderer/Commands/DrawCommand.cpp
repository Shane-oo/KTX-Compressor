//
// Created by shane on 6/08/2024.
//

#include "DrawCommand.h"
#include "../Graphics/GraphicsPipeline.h"
#include "../RendererConstants.h"

namespace KTXCompressor {

    // #region Private Methods

    vector<VkCommandBuffer> DrawCommand::CreateVulkanCommandBuffers() {
        VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.commandPool = vulkanCommandPool;
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandBufferCount = 1;

        vector<VkCommandBuffer> commandBuffers;
        commandBuffers.resize(RendererConstants::MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < RendererConstants::MAX_FRAMES_IN_FLIGHT; i++) {
            VkResult allocateCommandBuffersResult = vkAllocateCommandBuffers(logicalDevice->GetVulkanDevice(),
                                                                             &commandBufferAllocateInfo,
                                                                             &commandBuffers[i]);

            if (allocateCommandBuffersResult != VK_SUCCESS) {
                throw runtime_error("Failed to Allocate Command Buffers For Draw Command");
            }

            cout << "Successfully Allocated Draw Command Buffer " << i << endl;
        }

        return commandBuffers;
    }

    // #endregion

    // #region Constructors

    DrawCommand::DrawCommand(LogicalDevice *logicalDevice) : Command(logicalDevice) {
        vulkanDrawCommandBuffers = CreateVulkanCommandBuffers();
    }

    // #endregion

    // #region Destructors

    DrawCommand::~DrawCommand() {
        cout << "Destroy Draw Command" << endl;

        vkDestroyCommandPool(logicalDevice->GetVulkanDevice(), vulkanCommandPool, nullptr);
    }

    // #endregion

    // #region Public Methods

    void DrawCommand::Begin(uint32_t currentFrame) {
        // reset to make sure it can be recorded to
        vkResetCommandBuffer(vulkanDrawCommandBuffers[currentFrame], 0);

        VkCommandBufferBeginInfo commandBufferBeginInfo = {};
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags = 0; // Optional
        commandBufferBeginInfo.pInheritanceInfo = nullptr; // Optional

        VkResult beginCommandBufferResult = vkBeginCommandBuffer(vulkanDrawCommandBuffers[currentFrame],
                                                                 &commandBufferBeginInfo);

        if (beginCommandBufferResult != VK_SUCCESS) {
            throw runtime_error("Failed to Begin Recording Draw Command Buffer " + to_string(currentFrame));
        }
    }

    void DrawCommand::End(uint32_t currentFrame) {
        VkResult endCommandBufferResult = vkEndCommandBuffer(vulkanDrawCommandBuffers[currentFrame]);

        if (endCommandBufferResult != VK_SUCCESS) {
            throw runtime_error("Failed To End Command Buffer " + to_string(currentFrame));
        }

        //cout << "Successfully Ended Command Buffer" << endl;
    }

    // #endregion

} // KTXCompressor