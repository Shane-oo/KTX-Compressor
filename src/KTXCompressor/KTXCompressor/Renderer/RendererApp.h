//
// Created by shane on 6/07/2024.
//

#ifndef KTXCOMPRESSOR_RENDERERAPP_H
#define KTXCOMPRESSOR_RENDERERAPP_H

#include "../Common.h"
#include "Presentation/Window.h"
#include "Instance/Instance.h"
#include "Instance/Debug/Debugger.h"
#include "Presentation/SwapChain.h"
#include "Synchronization/Synchronization.h"

namespace KTXCompressor {

    class RendererApp {
    public:
        RendererApp();

        ~RendererApp();

    public:
        void Run();

    private:
        bool render = false;
        Window *window;
        Instance *instance;
        PhysicalDevice *physicalDevice;
        LogicalDevice *logicalDevice;
        SwapChain *swapChain;
        vector<GraphicsPipeline *> graphicsPipelines;
        Synchronization *synchronization;
        uint32_t currentFrame = 0;
        GraphicsPipeline *newGraphicsPipeline = nullptr;

        void MainLoop();

        void DrawFrame();
    };
} // KTXCompressor


#endif //KTXCOMPRESSOR_RENDERERAPP_H
