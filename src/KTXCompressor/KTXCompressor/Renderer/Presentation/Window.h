//
// Created by shane on 6/07/2024.
//

#ifndef KTXCOMPRESSOR_WINDOW_H
#define KTXCOMPRESSOR_WINDOW_H

#define GLFW_INCLUDE_VULKAN

#include "../../Common.h"
#include "../Instance/Instance.h"
#include <GLFW/glfw3.h>

namespace KTXCompressor {

    class Window {
    public:
        Window();

        ~Window();

    private:
        GLFWwindow *glfwWindow;
        Instance *instance;
        VkSurfaceKHR vulkanSurface;

    public:
        bool GetWindowShouldClose();

    private:
        static GLFWwindow *CreateGLFWWindow();

        VkSurfaceKHR CreateSurface();

    public:
        void SetInstance(Instance *pInstance);


        VkSurfaceKHR GetVulkanSurface() {
            return vulkanSurface;
        }
    };
} // KTXCompressor

#endif //KTXCOMPRESSOR_WINDOW_H
