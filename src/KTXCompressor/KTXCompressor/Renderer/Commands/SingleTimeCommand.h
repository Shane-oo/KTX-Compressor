//
// Created by ShaneMonck on 10/08/2024.
//

#ifndef KTX_COMPRESSOR_SINGLETIMECOMMAND_H
#define KTX_COMPRESSOR_SINGLETIMECOMMAND_H

#include "Command.h"

namespace KTXCompressor {

    class SingleTimeCommand : public Command {

    public:
        SingleTimeCommand(LogicalDevice *logicalDevice);

        ~SingleTimeCommand();

        void Copy(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        void CopyImage(VkImage image,
                       VkFormat format,
                       VkBuffer stagingBuffer,
                       uint32_t width,
                       uint32_t height);

        void TransitionImage(VkImage image,
                             VkFormat format,
                             VkImageLayout oldLayout,
                             VkImageLayout newLayout);


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
                                   VkFormat format,
                                   VkImageMemoryBarrier& imageMemoryBarrier,
                                   VkPipelineStageFlags &sourceStage,
                                   VkPipelineStageFlags &destinationStage);
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_SINGLETIMECOMMAND_H
