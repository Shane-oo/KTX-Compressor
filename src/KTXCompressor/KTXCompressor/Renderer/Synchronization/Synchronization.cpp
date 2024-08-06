//
// Created by shane on 6/08/2024.
//

#include "Synchronization.h"

namespace KTXCompressor {
    // #region Private Methods

    VkSemaphore Synchronization::CreateSemaphore() {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkSemaphore semaphore;
        VkResult createSemaphoreResult = vkCreateSemaphore(vulkanDevice, &semaphoreCreateInfo, nullptr, &semaphore);

        if (createSemaphoreResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Semaphore");
        }

        cout << "Successfully Created Semaphore" << endl;

        return semaphore;
    }

    VkFence Synchronization::CreateFence() {
        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        VkFence fence;
        VkResult createFenceResult = vkCreateFence(vulkanDevice, &fenceCreateInfo, nullptr, &fence);

        if (createFenceResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Fence");
        }

        cout << "Successfully Created Fence" << endl;

        return fence;
    }

    // #endregion

    // #region Constructors

    Synchronization::Synchronization(VkDevice vulkanDevice) {
        this->vulkanDevice = vulkanDevice;

        imageAvailableSemaphore = CreateSemaphore();
        renderFinishedSemaphore = CreateSemaphore();
        inFlightFence = CreateFence();
    }

    // #endregion

    // #region Destructors 

    Synchronization::~Synchronization() {
        cout << "Destroy Synchronization" << endl;

        vkDestroySemaphore(vulkanDevice, imageAvailableSemaphore, nullptr);
        vkDestroySemaphore(vulkanDevice, renderFinishedSemaphore, nullptr);
        vkDestroyFence(vulkanDevice, inFlightFence, nullptr);
    }

    // #endregion

    // #region Public Methods

    void Synchronization::WaitForFences() {
        // waits on the host for all fences to finish 
        vkWaitForFences(vulkanDevice, 1, &inFlightFence, VK_TRUE, UINT64_MAX);

        // reset to the unsignaled state
        vkResetFences(vulkanDevice, 1, &inFlightFence);
    }

    // #endregion

} // KTXCompressor