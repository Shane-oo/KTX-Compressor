//
// Created by shane on 6/07/2024.
//


#include "Window.h"

namespace KTXCompressor {

    // #region Constants

    static const uint32_t WIDTH = 800;
    static const uint32_t HEIGHT = 800;

    // #endregion


    // #region Private Methods 

    GLFWwindow *Window::CreateGLFWWindow() {
        glfwInit();

        // Do not create an OpenGL context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // Disable window resize for now
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        return glfwCreateWindow(WIDTH, HEIGHT, "KTX-Compressor", nullptr, nullptr);
    }

    // #endregion

    // #region Constructors

    Window::Window() {
        glfwWindow = CreateGLFWWindow();
    }

    // #engregion

    // #region Destructors


    Window::~Window() {
        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }

    // #endregion


    // #region Public Methods

    bool Window::GetWindowShouldClose() {
        bool shouldClose = glfwWindowShouldClose(glfwWindow);
        if (!shouldClose) {
            glfwPollEvents();
        }
        return shouldClose;
    }

    // #endregion
} // KTXCompressor