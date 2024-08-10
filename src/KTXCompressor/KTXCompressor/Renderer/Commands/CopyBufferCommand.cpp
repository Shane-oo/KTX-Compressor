//
// Created by ShaneMonck on 10/08/2024.
//

#include "CopyBufferCommand.h"

namespace KTXCompressor {

    // #region Private Methods

    VkCommandBuffer CopyBufferCommand::CreateCopyCommandBuffer() {
        return nullptr;
    }

    // #endregion

    // #region Constructors

    CopyBufferCommand::CopyBufferCommand(VkDevice vulkanDevice, uint32_t graphicsFamilyIndex) : Command(vulkanDevice,
                                                                                                        graphicsFamilyIndex) {
        vulkanCopyCommandBuffer = CreateCopyCommandBuffer();
    }


    // #endregion

    // #region Destructors   
    CopyBufferCommand::~CopyBufferCommand() {
        //todo
    }


    // #endregion

} // KTXCompressor