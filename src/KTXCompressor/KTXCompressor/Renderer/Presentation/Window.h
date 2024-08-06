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
        bool frameBufferResized = false;

    public:
        bool GetWindowShouldClose();

    private:
        GLFWwindow *CreateGLFWWindow();

        VkSurfaceKHR CreateSurface();
        
        static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height);

    public:
        void SetInstance(Instance *pInstance);


        VkSurfaceKHR GetVulkanSurface() {
            return vulkanSurface;
        }

        GLFWwindow *GetGlfwWindow() {
            return glfwWindow;
        }

        bool GetFrameBufferResized() {
            return frameBufferResized;
        }

        void SetFrameBufferResized(bool resized) {
            frameBufferResized = resized;
        }
        
        bool IsMinimised();
    };
} // KTXCompressor

#endif //KTXCOMPRESSOR_WINDOW_H
