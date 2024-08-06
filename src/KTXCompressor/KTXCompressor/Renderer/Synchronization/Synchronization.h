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
        vector<VkSemaphore> imageAvailableSemaphores;
        vector<VkSemaphore> renderFinishedSemaphores;
        vector<VkFence> inFlightFences;

        vector<VkSemaphore> CreateSemaphores();

        vector<VkFence> CreateFences();

    public:
        VkSemaphore GetWaitSemaphore(uint32_t currentFrame) {
            return imageAvailableSemaphores[currentFrame];
        }

        VkSemaphore GetSignalSemaphore(uint32_t currentFrame) {
            return renderFinishedSemaphores[currentFrame];
        }

        VkFence GetInFlightFence(uint32_t currentFrame) {
            return inFlightFences[currentFrame];
        }

    public:
        void WaitForFences(uint32_t currentFrame);
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_SYNCHRONIZATION_H
