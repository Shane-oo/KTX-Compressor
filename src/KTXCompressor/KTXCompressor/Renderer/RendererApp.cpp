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
        // TODO: How can i render multiple renderPasses to the one swap chain present image/framebuffe?

        synchronization->WaitForFence(currentFrame);

        bool nextImageReady = swapChain->NextImage(synchronization->GetWaitSemaphore(currentFrame));
        if (!nextImageReady) {
            return;
        }
        synchronization->ResetFence(currentFrame);


        vector<VkCommandBuffer> drawCommands;
        for (size_t i = 0; i < graphicsPipelines.size(); i++) {
            // Get the correct framebuffer for the current pipeline
            VkFramebuffer vulkanFrameBuffer = swapChain->GetFramebufferForGraphicsPipeline(i);

            // Draw using the current pipeline and its corresponding framebuffer
            auto drawCommand = graphicsPipelines[i]->Draw(vulkanFrameBuffer, currentFrame);

            drawCommands.push_back(drawCommand);
        }

        swapChain->Submit(synchronization, currentFrame, drawCommands);

        swapChain->Present(synchronization, currentFrame);

        currentFrame = (currentFrame + 1) % RendererConstants::MAX_FRAMES_IN_FLIGHT;

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

        // todo a better way of setting each ones index...
        graphicsPipelines.push_back(new SimpleTriangleGraphicsPipeline(physicalDevice,
                                                                       logicalDevice,
                                                                       swapChain,
                                                                       physicalDevice->GetGraphicsFamilyIndex(),
                                                                       true,
                                                                       1));
        graphicsPipelines.push_back(new SimpleTriangleGraphicsPipeline(physicalDevice,
                                                                       logicalDevice,
                                                                       swapChain,
                                                                       physicalDevice->GetGraphicsFamilyIndex(),
                                                                       false,
                                                                       2));
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