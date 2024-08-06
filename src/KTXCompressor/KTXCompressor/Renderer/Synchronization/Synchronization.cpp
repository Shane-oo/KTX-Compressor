//
// Created by shane on 6/08/2024.
//

#include "Synchronization.h"
#include "../RendererConstants.h"

namespace KTXCompressor {
    // #region Private Methods

    vector<VkSemaphore> Synchronization::CreateSemaphores() {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        vector<VkSemaphore> semaphores;
        semaphores.resize(RendererConstants::MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < RendererConstants::MAX_FRAMES_IN_FLIGHT; i++) {
            VkResult createSemaphoreResult = vkCreateSemaphore(vulkanDevice,
                                                               &semaphoreCreateInfo,
                                                               nullptr,
                                                               &semaphores[i]);

            if (createSemaphoreResult != VK_SUCCESS) {
                throw runtime_error("Failed to Create Semaphores");
            }

            cout << "Successfully Created Semaphore " << i << endl;

        }

        return semaphores;
    }

    vector<VkFence> Synchronization::CreateFences() {
        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        vector<VkFence> fences;
        fences.resize(RendererConstants::MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < RendererConstants::MAX_FRAMES_IN_FLIGHT; i++) {
            VkResult createFenceResult = vkCreateFence(vulkanDevice,
                                                       &fenceCreateInfo,
                                                       nullptr,
                                                       &fences[i]);

            if (createFenceResult != VK_SUCCESS) {
                throw runtime_error("Failed to Create Fences");
            }

            cout << "Successfully Created Fence " << i << endl;

        }

        return fences;
    }

    // #endregion

    // #region Constructors

    Synchronization::Synchronization(VkDevice vulkanDevice) {
        this->vulkanDevice = vulkanDevice;

        imageAvailableSemaphores = CreateSemaphores();
        renderFinishedSemaphores = CreateSemaphores();
        inFlightFences = CreateFences();
    }

    // #endregion

    // #region Destructors 

    Synchronization::~Synchronization() {
        cout << "Destroy Synchronization" << endl;

        for (auto imageAvailableSemaphore: imageAvailableSemaphores) {
            vkDestroySemaphore(vulkanDevice, imageAvailableSemaphore, nullptr);
        }
        for (auto renderFinishedSemaphore: renderFinishedSemaphores) {
            vkDestroySemaphore(vulkanDevice, renderFinishedSemaphore, nullptr);
        }
        for (auto inFlightFence: inFlightFences) {
            vkDestroyFence(vulkanDevice, inFlightFence, nullptr);
        }
    }

    // #endregion

    // #region Public Methods

    void Synchronization::WaitForFence(uint32_t currentFrame) {
        // waits on the host for all fences to finish 
        vkWaitForFences(vulkanDevice, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    }

    void Synchronization::ResetFence(uint32_t currentFrame) {
        // reset to the unsignaled state
        vkResetFences(vulkanDevice, 1, &inFlightFences[currentFrame]);
    }

    // #endregion

} // KTXCompressor