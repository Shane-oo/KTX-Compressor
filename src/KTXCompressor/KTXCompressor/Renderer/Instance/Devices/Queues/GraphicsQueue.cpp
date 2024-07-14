//
// Created by shane on 14/07/2024.
//

#include "GraphicsQueue.h"

namespace KTXCompressor {

    // #region Constructors

    GraphicsQueue::GraphicsQueue(VkQueue vkQueue) {
        vulkanQueue = vkQueue;
    }

    // #endregion

} // KTXCompressor