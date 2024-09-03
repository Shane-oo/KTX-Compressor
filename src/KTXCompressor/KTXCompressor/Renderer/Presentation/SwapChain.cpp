//
// Created by shane on 19/07/2024.
//


#include <GLFW/glfw3.h>
#include "SwapChain.h"
#include "../Graphics/GraphicsPipeline.h"

namespace KTXCompressor {

    // #region Private Methods

    VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR> &availableFormats) {
        // Looking for the color space SRGB, default to first available if not found
        VkSurfaceFormatKHR chosenFormat = availableFormats[0];

        for (const auto &availableFormat: availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB
                && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                chosenFormat = availableFormat;

                cout << "Using VK_FORMAT_B8G8R8A8_SRGB For Swap Surface Format" << endl;

            }
        }


        return chosenFormat;
    }

    VkPresentModeKHR SwapChain::ChooseSwapPresentMode(const vector<VkPresentModeKHR> &availablePresentModes) {
        VkPresentModeKHR chosenPresentMode = VK_PRESENT_MODE_FIFO_KHR;

        for (const auto &availablePresentMode: availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                chosenPresentMode = availablePresentMode;

                cout << "Using VK_PRESENT_MODE_MAILBOX_KHR For Swap Present Mode" << endl;
            }
        }


        return chosenPresentMode;
    }

    VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
        if (capabilities.currentExtent.width != numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        } else {
            int width, height;
            glfwGetFramebufferSize(window->GetGlfwWindow(), &width, &height);

            VkExtent2D actualExtent = {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
            };

            actualExtent.width = clamp(actualExtent.width,
                                       capabilities.minImageExtent.width,
                                       capabilities.maxImageExtent.width);
            actualExtent.height = clamp(actualExtent.height,
                                        capabilities.minImageExtent.height,
                                        capabilities.maxImageExtent.height);

            return actualExtent;

        }
    }

    VkSwapchainKHR SwapChain::CreateVulkanSwapChain() {
        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice->GetVulkanPhysicalDevice(),
                                                                         this->window->GetVulkanSurface());

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
        imageFormat = surfaceFormat.format;
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        extent = ChooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0
            && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR swapChainCreateInfo{};
        swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapChainCreateInfo.surface = this->window->GetVulkanSurface();
        swapChainCreateInfo.minImageCount = imageCount;
        swapChainCreateInfo.imageFormat = surfaceFormat.format;
        swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapChainCreateInfo.imageExtent = extent;
        swapChainCreateInfo.imageArrayLayers = 1;
        swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;


        if (physicalDevice->GetGraphicsFamilyIndex() != physicalDevice->GetPresentFamilyIndex()) {
            // images can be used across multiple queue families without explicit ownership transfers 
            swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapChainCreateInfo.queueFamilyIndexCount = 2;
            uint32_t queueFamilyIndices[] = {physicalDevice->GetGraphicsFamilyIndex(),
                                             physicalDevice->GetPresentFamilyIndex()};
            swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            // an image is owned by one queue family at a time and ownership must be explicitly transferred before
            // using it in another queue family
            swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapChainCreateInfo.queueFamilyIndexCount = 0; // Optional
            swapChainCreateInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        // No transformations
        swapChainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;

        swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        swapChainCreateInfo.presentMode = presentMode;
        swapChainCreateInfo.clipped = VK_TRUE;
        swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

        VkSwapchainKHR swapChain;
        VkResult createSwapChainResult = vkCreateSwapchainKHR(logicalDevice->GetVulkanDevice(),
                                                              &swapChainCreateInfo,
                                                              nullptr,
                                                              &swapChain);
        if (createSwapChainResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Swap Chain");
        }


        return swapChain;
    }

    void SwapChain::RetrieveSwapChainImages() {
        uint32_t imageCount;
        vkGetSwapchainImagesKHR(logicalDevice->GetVulkanDevice(), vulkanSwapChain, &imageCount, nullptr);
        images.resize(imageCount);

        vkGetSwapchainImagesKHR(logicalDevice->GetVulkanDevice(), vulkanSwapChain, &imageCount, images.data());

        cout << "Retrieved " << images.size() << " Images From Swap Chain" << endl;
    }

    vector<ImageView *> *SwapChain::CreateImageViews() {
        RetrieveSwapChainImages();
        auto *views = new vector<ImageView *>();

        for (auto &image: images) {
            views->push_back(new ImageView(logicalDevice->GetVulkanDevice(), image, imageFormat, VK_IMAGE_ASPECT_COLOR_BIT));
        }

        return views;
    }

    vector<FrameBuffer *> *SwapChain::CreateFrameBuffers() {
        auto *fBuffers = new vector<FrameBuffer *>();

        for (auto imageView: *imageViews) {
            auto attachments = new vector<VkImageView>();
            attachments->push_back(imageView->GetVulkanImageView());

            auto *renderPass = graphicsPipeline->GetRenderPass();

            fBuffers->push_back(
                    new FrameBuffer(logicalDevice->GetVulkanDevice(),
                                    *attachments,
                                    renderPass->GetVulkanRenderPass(),
                                    extent)
            );
        }

        return fBuffers;
    }

    void SwapChain::RecreateVulkanSwapChain() {
        while (window->IsMinimised()) {}

        cout << "Recreating Vulkan Swap Chain" << endl;

        vkDeviceWaitIdle(logicalDevice->GetVulkanDevice());

        CleanUpVulkanSwapChain();

        vulkanSwapChain = CreateVulkanSwapChain();
        imageViews = CreateImageViews();
        frameBuffers = CreateFrameBuffers();
    }

    void SwapChain::CleanUpVulkanSwapChain() {
        for (auto frameBuffer: *frameBuffers) {
            delete frameBuffer;
        }
        delete frameBuffers;

        for (auto imageView: *imageViews) {
            delete imageView;
        }
        delete imageViews;

        vkDestroySwapchainKHR(logicalDevice->GetVulkanDevice(), vulkanSwapChain, nullptr);
    }

    // #endregion

    // #region Constructors

    SwapChain::SwapChain(PhysicalDevice *physicalDevice, Window *window, LogicalDevice *logicalDevice) {
        this->window = window;
        this->logicalDevice = logicalDevice;
        this->physicalDevice = physicalDevice;
        vulkanSwapChain = CreateVulkanSwapChain();
        imageViews = CreateImageViews();
    }

    // #endregion

    // #region Destructors

    SwapChain::~SwapChain() {
        cout << "Destroy Swap Chain" << endl;

        CleanUpVulkanSwapChain();
    }

    // #endregion

    // #region Public Methods

    SwapChain::SwapChainSupportDetails
    SwapChain::QuerySwapChainSupport(VkPhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR vulkanSurface) {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkanPhysicalDevice, vulkanSurface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(vulkanPhysicalDevice, vulkanSurface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(vulkanPhysicalDevice,
                                                 vulkanSurface,
                                                 &formatCount,
                                                 details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(vulkanPhysicalDevice, vulkanSurface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(vulkanPhysicalDevice,
                                                      vulkanSurface,
                                                      &presentModeCount,
                                                      details.presentModes.data());
        }


        return details;
    }

    void SwapChain::SetGraphicsPipeline(GraphicsPipeline *pGraphicsPipeline) {
        this->graphicsPipeline = pGraphicsPipeline;
        frameBuffers = CreateFrameBuffers();
    }

    VkFramebuffer SwapChain::NextImage(VkSemaphore imageAvailableSemaphore) {
        VkResult acquireNextImageResult = vkAcquireNextImageKHR(logicalDevice->GetVulkanDevice(),
                                                                vulkanSwapChain,
                                                                UINT64_MAX,
                                                                imageAvailableSemaphore,
                                                                VK_NULL_HANDLE,
                                                                &imageIndex);

        if (acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR) {
            RecreateVulkanSwapChain();
            return nullptr;
        }

        if (acquireNextImageResult != VK_SUCCESS && acquireNextImageResult != VK_SUBOPTIMAL_KHR) {
            throw runtime_error("Failed to Acquire Swap Chain Image!");
        }

        return (*frameBuffers)[imageIndex]->GetVulkanFrameBuffer();
    }

    void SwapChain::Present(Synchronization *synchronization, uint32_t currentFrame) {
        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;


        VkSemaphore signalSemaphores[] = {synchronization->GetSignalSemaphore(currentFrame)};
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {vulkanSwapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex; // note is set in NextImage
        presentInfo.pResults = nullptr;

        auto presentQueue = logicalDevice->GetPresentQueue();
        VkResult presentResult = vkQueuePresentKHR(presentQueue->GetVulkanQueue(), &presentInfo);

        if (presentResult == VK_ERROR_OUT_OF_DATE_KHR
            || presentResult == VK_SUBOPTIMAL_KHR
            || window->GetFrameBufferResized()) {

            window->SetFrameBufferResized(false);
            RecreateVulkanSwapChain();
            return;
        }
        if (presentResult != VK_SUCCESS) {
            throw runtime_error("Failed to Present Swap Chain Image");
        }
    }


    // #endregion


} // KTXCompressor