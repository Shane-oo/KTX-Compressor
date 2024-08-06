//
// Created by shane on 6/08/2024.
//

#ifndef KTXCOMPRESSOR_SYNCHRONIZATION_H
#define KTXCOMPRESSOR_SYNCHRONIZATION_H

#include <vulkan/vulkan_core.h>
#include "../../Common.h"

namespace KTXCompressor {

    class Synchronization {
    public:
        Synchronization(VkDevice vulkanDevice);

        ~Synchronization();

    private:
        VkDevice vulkanDevice;
        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        VkFence inFlightFence;

        VkSemaphore CreateSemaphore();

        VkFence CreateFence();

    public:
        VkSemaphore GetWaitSemaphore() {
            return imageAvailableSemaphore;
        }

        VkSemaphore GetSignalSemaphore() {
            return renderFinishedSemaphore;
        }

        VkFence GetInFlightFence() {
            return inFlightFence;
        }

    public:
        void WaitForFences();
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_SYNCHRONIZATION_H
