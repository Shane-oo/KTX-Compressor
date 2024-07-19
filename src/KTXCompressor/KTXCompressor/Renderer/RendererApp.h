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

namespace KTXCompressor {

    class RendererApp {
    public:
        RendererApp();

        ~RendererApp();

    public:
        void Run();

    private:
        Window *window;
        Instance *instance;
        PhysicalDevice *physicalDevice;
        LogicalDevice *logicalDevice;
        SwapChain *swapChain;

        void MainLoop();
    };
} // KTXCompressor


#endif //KTXCOMPRESSOR_RENDERERAPP_H
