//
// Created by shane on 19/07/2024.
//

#ifndef KTXCOMPRESSOR_SWAPCHAIN_H
#define KTXCOMPRESSOR_SWAPCHAIN_H

#include <vulkan/vulkan_core.h>
#include "../../Common.h"
#include "Window.h"
#include "ImageView.h"


namespace KTXCompressor {

    class SwapChain {
    public:
        SwapChain(PhysicalDevice *physicalDevice, Window *window, LogicalDevice *logicalDevice);

        ~SwapChain();

    private:
        Window *window;
        LogicalDevice *logicalDevice;
        vector<ImageView *> *imageViews;
        VkSwapchainKHR vulkanSwapChain;
        vector<VkImage> images;
        VkFormat imageFormat;
        VkExtent2D extent;

        struct SwapChainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            vector<VkSurfaceFormatKHR> formats;
            vector<VkPresentModeKHR> presentModes;

            [[nodiscard]] bool IsAdequate() const {
                return !formats.empty() && !presentModes.empty();
            }
        };

        vector<ImageView *> *CreateImageViews();

        void RetrieveSwapChainImages();

        VkSwapchainKHR CreateVulkanSwapChain(PhysicalDevice *physicalDevice);

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR> &availableFormats);

        VkPresentModeKHR ChooseSwapPresentMode(const vector<VkPresentModeKHR> &availablePresentModes);

        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    public:
        static SwapChainSupportDetails
        QuerySwapChainSupport(VkPhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR vulkanSurface);

    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_SWAPCHAIN_H
