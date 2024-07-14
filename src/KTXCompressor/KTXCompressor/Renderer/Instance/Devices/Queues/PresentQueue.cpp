//
// Created by shane on 14/07/2024.
//

#include "PresentQueue.h"

namespace KTXCompressor {

    // #region Constructors

    PresentQueue::PresentQueue(VkQueue vkQueue) {
        vulkanQueue = vkQueue;
    }

    // #endregion

} // KTXCompressor