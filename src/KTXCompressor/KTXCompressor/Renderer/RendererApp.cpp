//
// Created by Shane Monck on 6/07/2024.
//


#include "RendererApp.h"
#include "Graphics/SimpleTriangle/SimpleTriangleGraphicsPipeline.h"
#include "RendererConstants.h"

namespace KTXCompressor {

    // #region Private Methods

    void RendererApp::MainLoop() {
        while (!window->GetWindowShouldClose()) {
            DrawFrame();
        }

        vkDeviceWaitIdle(logicalDevice->GetVulkanDevice());
    }

    void RendererApp::DrawFrame() {
        for (size_t i = 0; i < graphicsPipelines.size(); i++) {
            synchronization->WaitForFence(currentFrame);

            auto vulkanFrameBuffer = swapChain->NextImage(synchronization->GetWaitSemaphore(currentFrame), i);

            if (!vulkanFrameBuffer) {
                return;
            }

            synchronization->ResetFence(currentFrame);

            graphicsPipelines[i]->Draw(vulkanFrameBuffer, currentFrame);

            graphicsPipelines[i]->Submit(synchronization, currentFrame);

            swapChain->Present(synchronization, currentFrame);

            currentFrame = (currentFrame + 1) % RendererConstants::MAX_FRAMES_IN_FLIGHT;
        }
        
        /*TODO Implement something like this maybe*/
        /*void RendererApp::DrawFrame() {
    synchronization->WaitForFence(currentFrame);

    // Acquire the next image from the swapchain
    VkFramebuffer vulkanFrameBuffer = nullptr;
    VkResult acquireResult = swapChain->AcquireNextImage(synchronization->GetWaitSemaphore(currentFrame));
    if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR || acquireResult == VK_SUBOPTIMAL_KHR) {
        RecreateSwapChain();
        return;
    } else if (acquireResult != VK_SUCCESS) {
        throw std::runtime_error("Failed to acquire swapchain image!");
    }

    for (size_t i = 0; i < graphicsPipelines.size(); i++) {
        // Get the correct framebuffer for the current pipeline
        vulkanFrameBuffer = swapChain->GetFramebufferForPipeline(i);

        // Draw using the current pipeline and its corresponding framebuffer
        graphicsPipelines[i]->Draw(vulkanFrameBuffer, currentFrame);
    }

    // Submit commands and present the image
    for (size_t i = 0; i < graphicsPipelines.size(); i++) {
        graphicsPipelines[i]->Submit(synchronization, currentFrame);
    }

    swapChain->Present(synchronization, currentFrame);

    // Reset the fence and advance to the next frame
    synchronization->ResetFence(currentFrame);
    currentFrame = (currentFrame + 1) % RendererConstants::MAX_FRAMES_IN_FLIGHT;
}*/
        
        //cout << currentFrame << endl;
    }

    // #endregion

    // #region Constructors

    RendererApp::RendererApp() {
        window = new Window();
        instance = new Instance();
        window->SetInstance(instance);

        auto *queueFamily = new QueueFamily(window->GetVulkanSurface());
        physicalDevice = new PhysicalDevice(instance->GetVulkanInstance(), queueFamily);

        // todo Logical Device should potentially live inside Physcial vulkanDevice? "Logical devices don’t interact directly with instance"
        logicalDevice = new LogicalDevice(physicalDevice);

        swapChain = new SwapChain(physicalDevice, window, logicalDevice);

        graphicsPipelines.push_back(new SimpleTriangleGraphicsPipeline(physicalDevice,
                                                                       logicalDevice,
                                                                       swapChain,
                                                                       physicalDevice->GetGraphicsFamilyIndex()));
        // todo undo this...
        graphicsPipelines.push_back(new SimpleTriangleGraphicsPipeline(physicalDevice,
                                                                       logicalDevice,
                                                                       swapChain,
                                                                       physicalDevice->GetGraphicsFamilyIndex()));
        // SetGraphicsPipelines
        swapChain->SetGraphicsPipelines(graphicsPipelines);

        synchronization = new Synchronization(logicalDevice->GetVulkanDevice());
    }

    // #endregion

    // #region Destructors

    RendererApp::~RendererApp() {
        cout << "Destroy Renderer App" << endl;

        delete synchronization;
        for (auto graphicsPipeline: graphicsPipelines) {
            delete graphicsPipeline;
        }
        delete swapChain;
        delete logicalDevice;
        delete physicalDevice;
        delete window;
        delete instance;
    }

    // #endregion

    // #region Public Methods

    void RendererApp::Run() {
        MainLoop();
    }


    // #endregion

} // KTXCompressor