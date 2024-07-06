//
// Created by shane on 6/07/2024.
//

#ifndef KTXCOMPRESSOR_RENDERERAPP_H
#define KTXCOMPRESSOR_RENDERERAPP_H

#include "../Common.h"
#include "Window.h"


class RendererApp {
public:
    void Run();

private:
    Window *window;

    void InitVulkan();

    void MainLoop();

    void Destroy();
};


#endif //KTXCOMPRESSOR_RENDERERAPP_H
