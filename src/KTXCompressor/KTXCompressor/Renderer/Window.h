//
// Created by shane on 6/07/2024.
//

#ifndef KTXCOMPRESSOR_WINDOW_H
#define KTXCOMPRESSOR_WINDOW_H

#define GLFW_INCLUDE_VULKAN

#include "../Common.h"
#include <GLFW/glfw3.h>

class Window {
public:
    Window();

    ~Window();

private:
    GLFWwindow *glfwWindow;

public:
    bool GetWindowShouldClose();

private:
    static GLFWwindow *CreateGLFWWindow();
};


#endif //KTXCOMPRESSOR_WINDOW_H
