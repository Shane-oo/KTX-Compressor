//
// Created by ShaneMonck on 10/08/2024.
//

#ifndef KTX_COMPRESSOR_COPYBUFFERCOMMAND_H
#define KTX_COMPRESSOR_COPYBUFFERCOMMAND_H

#include "Command.h"

namespace KTXCompressor {

    class CopyBufferCommand : public Command {

    public:
        CopyBufferCommand(LogicalDevice *logicalDevice);

        ~CopyBufferCommand();

        void Copy(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        void CopyImage(VkImage image,
                       VkFormat format,
                       VkBuffer stagingBuffer,
                       uint32_t width,
                       uint32_t height);


    private:
        VkCommandBuffer vulkanCommandBuffer;

        VkCommandBuffer CreatCommandBuffer();

        void Begin();

        void CopyBuffers(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        void End();

        void Submit();

        void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

        void Transition(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

        void GetPipelineStageFlags(VkImageLayout oldLayout,
                                   VkImageLayout newLayout,
                                   VkImageMemoryBarrier& imageMemoryBarrier,
                                   VkPipelineStageFlags &sourceStage,
                                   VkPipelineStageFlags &destinationStage);
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_COPYBUFFERCOMMAND_H
