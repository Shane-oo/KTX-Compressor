//
// Created by ShaneMonck on 10/08/2024.
//

#ifndef KTX_COMPRESSOR_COPYBUFFERCOMMAND_H
#define KTX_COMPRESSOR_COPYBUFFERCOMMAND_H

#include "Command.h"

namespace KTXCompressor {

    class CopyBufferCommand : public Command {

    public:
        CopyBufferCommand(LogicalDevice *logicalDevice, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        ~CopyBufferCommand();


    private:
        VkCommandBuffer vulkanCopyCommandBuffer;

        VkCommandBuffer CreateCopyCommandBuffer();

        void Begin();

        void Copy(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        void End();

        void Submit();
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_COPYBUFFERCOMMAND_H
