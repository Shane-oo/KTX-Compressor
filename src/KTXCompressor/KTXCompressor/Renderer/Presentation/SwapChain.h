//
// Created by shane on 19/07/2024.
//

#ifndef KTXCOMPRESSOR_SWAPCHAIN_H
#define KTXCOMPRESSOR_SWAPCHAIN_H

#include <vulkan/vulkan_core.h>
#include "../../Common.h"
#include "Window.h"
#include "ImageView.h"
#include "FrameBuffer.h"
#include "../Synchronization/Synchronization.h"


namespace KTXCompressor {

    class GraphicsPipeline;

    class SwapChain {
    public:
        SwapChain(PhysicalDevice *physicalDevice, Window *window, LogicalDevice *logicalDevice);

        ~SwapChain();

    private:
        Window *window;
        LogicalDevice *logicalDevice;
        GraphicsPipeline *graphicsPipeline;
        vector<ImageView *> *imageViews;
        vector<FrameBuffer *> *frameBuffers;
        VkSwapchainKHR vulkanSwapChain;
        vector<VkImage> images;
        VkFormat imageFormat;
        VkExtent2D extent;
        uint32_t imageIndex;

        struct SwapChainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            vector<VkSurfaceFormatKHR> formats;
            vector<VkPresentModeKHR> presentModes;

            [[nodiscard]] bool IsAdequate() const {
                return !formats.empty() && !presentModes.empty();
            }
        };

        vector<ImageView *> *CreateImageViews();

        vector<FrameBuffer *> *CreateFrameBuffers();

        void RetrieveSwapChainImages();

        VkSwapchainKHR CreateVulkanSwapChain(PhysicalDevice *physicalDevice);

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR> &availableFormats);

        VkPresentModeKHR ChooseSwapPresentMode(const vector<VkPresentModeKHR> &availablePresentModes);

        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    public:
        static SwapChainSupportDetails
        QuerySwapChainSupport(VkPhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR vulkanSurface);

        VkFramebuffer NextImage(VkSemaphore imageAvailableSemaphore);
        
        void Present(Synchronization *synchronization, uint32_t currentFrame);

    public:
        VkExtent2D GetExtent() {
            return extent;
        }

        VkFormat GetImageFormat() {
            return imageFormat;
        }

        void SetGraphicsPipeline(GraphicsPipeline *pGraphicsPipeline);
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_SWAPCHAIN_H
