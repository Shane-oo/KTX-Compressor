//
// Created by shane on 14/07/2024.
//

#include "Queue.h"

namespace KTXCompressor {

    // #region Constructors

    Queue::Queue(VkQueue vkQueue, uint32_t queueFamilyIndex) {
        vulkanQueue = vkQueue;
        this->queueFamilyIndex = queueFamilyIndex;
    }

    // #endregion

} // KTXCompressor