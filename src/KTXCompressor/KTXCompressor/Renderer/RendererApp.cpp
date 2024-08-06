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
        synchronization->WaitForFences(currentFrame);

        auto vulkanFrameBuffer = swapChain->NextImage(synchronization->GetWaitSemaphore(currentFrame));

        graphicsPipeline->Draw(vulkanFrameBuffer, currentFrame);

        graphicsPipeline->Submit(synchronization, currentFrame);

        swapChain->Present(synchronization, currentFrame);

        currentFrame = (currentFrame + 1) % RendererConstants::MAX_FRAMES_IN_FLIGHT;

        cout << currentFrame << endl;
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

        graphicsPipeline = new SimpleTriangleGraphicsPipeline(logicalDevice,
                                                              swapChain,
                                                              physicalDevice->GetGraphicsFamilyIndex());

        swapChain->SetGraphicsPipeline(graphicsPipeline);

        synchronization = new Synchronization(logicalDevice->GetVulkanDevice());
    }

    // #endregion

    // #region Destructors

    RendererApp::~RendererApp() {
        cout << "Destroy RendererApp" << endl;

        delete synchronization;
        delete graphicsPipeline;
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