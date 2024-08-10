//
// Created by ShaneMonck on 10/08/2024.
//

#ifndef KTX_COMPRESSOR_COPYBUFFERCOMMAND_H
#define KTX_COMPRESSOR_COPYBUFFERCOMMAND_H

#include "Command.h"

namespace KTXCompressor {

    class CopyBufferCommand : public Command {
    public:
        CopyBufferCommand(VkDevice vulkanDevice, uint32_t graphicsFamilyIndex);

        ~CopyBufferCommand();

    private:
        VkCommandBuffer vulkanCopyCommandBuffer;
        
        VkCommandBuffer CreateCopyCommandBuffer();
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_COPYBUFFERCOMMAND_H
