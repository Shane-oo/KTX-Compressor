//
// Created by shane on 6/07/2024.
//

#ifndef KTXCOMPRESSOR_RENDERERAPP_H
#define KTXCOMPRESSOR_RENDERERAPP_H

#include "../Common.h"
#include "Window.h"
#include "Instance.h"

namespace KTXCompressor {

    class RendererApp {
    public:
        ~RendererApp();

    public:
        void Run();

    private:
        Window *window;
        Instance *instance;

        void InitVulkan();

        void MainLoop();

        void Destroy();
    };
} // KTXCompressor


#endif //KTXCOMPRESSOR_RENDERERAPP_H
