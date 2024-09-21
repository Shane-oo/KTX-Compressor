//
// Created by shane on 6/07/2024.
//


#include "Window.h"

namespace KTXCompressor {

    // #region Constants

    static const uint32_t WIDTH = 1000;
    static const uint32_t HEIGHT = 800;

    // #endregion


    // #region Private Methods 

    GLFWwindow *Window::CreateGLFWWindow() {
        glfwInit();

        // Do not create an OpenGL context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // Disable window resize for now
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        auto window = glfwCreateWindow(WIDTH, HEIGHT, "KTX-Compressor", nullptr, nullptr);

        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, FrameBufferResizeCallback);

        return window;
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

    void Window::FrameBufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto app = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        app->SetFrameBufferResized(true);
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

    bool Window::IsMinimised() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(glfwWindow, &width, &height);

        glfwWaitEvents();
        
        return width == 0 || height == 0;
    }


    // #endregion

} // KTXCompressor