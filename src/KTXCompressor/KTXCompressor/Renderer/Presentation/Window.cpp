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

    VkSurfaceKHR Window::CreateSurface() {
        VkSurfaceKHR surface;
        VkResult createSurfaceResult = glfwCreateWindowSurface(instance->GetVulkanInstance(),
                                                               glfwWindow,
                                                               nullptr,
                                                               &surface);
        if (createSurfaceResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Window Surface");
        }

        cout << "Successfully Created Window Surface" << endl;

        return surface;
    }

    // #endregion

    // #region Constructors

    Window::Window() {
        glfwWindow = CreateGLFWWindow();

        vulkanSurface = VK_NULL_HANDLE;
        instance = nullptr;
    }

    // #engregion

    // #region Destructors

    Window::~Window() {
        cout << "Destroy Window" << endl;

        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
        vkDestroySurfaceKHR(instance->GetVulkanInstance(), vulkanSurface, nullptr);
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

    void Window::SetInstance(Instance *pInstance) {
        instance = pInstance;
        vulkanSurface = CreateSurface();
    }

    // #endregion

} // KTXCompressor