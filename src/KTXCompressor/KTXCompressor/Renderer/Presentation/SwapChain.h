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
#include "../Textures/DepthTexture.h"


namespace KTXCompressor {

    class GraphicsPipeline;

    class SwapChain {
    public:
        SwapChain(PhysicalDevice *physicalDevice, Window *window, LogicalDevice *logicalDevice);

        ~SwapChain();

        void Submit(Synchronization *synchronization, uint32_t currentFrame, vector<VkCommandBuffer> allDrawCommands);

    private:
        Window *window;
        LogicalDevice *logicalDevice;
        PhysicalDevice *physicalDevice;
        vector<GraphicsPipeline *> graphicsPipelines;
        DepthTexture *depthTexture;
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

        VkSwapchainKHR CreateVulkanSwapChain();

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR> &availableFormats);

        VkPresentModeKHR ChooseSwapPresentMode(const vector<VkPresentModeKHR> &availablePresentModes);

        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        void RecreateVulkanSwapChain();

        void CleanUpVulkanSwapChain();

    public:
        static SwapChainSupportDetails
        QuerySwapChainSupport(VkPhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR vulkanSurface);

        bool NextImage(VkSemaphore imageAvailableSemaphore);

        VkFramebuffer GetFramebufferForGraphicsPipeline(size_t graphicsPipelineIndex);

        void Present(Synchronization *synchronization, uint32_t currentFrame);

    public:
        VkExtent2D GetExtent() {
            return extent;
        }

        VkFormat GetImageFormat() {
            return imageFormat;
        }

        void SetGraphicsPipelines(vector<GraphicsPipeline *> pipelines);

        uint32_t GetImagesCount() {
            return static_cast<uint32_t>(images.size());
        }
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_SWAPCHAIN_H
