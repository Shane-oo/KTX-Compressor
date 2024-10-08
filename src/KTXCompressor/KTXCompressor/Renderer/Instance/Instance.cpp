//
// Created by shane on 6/07/2024.
//

#include <GLFW/glfw3.h>
#include "Instance.h"
#include "../RendererConstants.h"

namespace KTXCompressor {

    // #region Constants

    static const vector<const char *> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };


    // #endregion

    // #region Private Methods

    VkInstance Instance::CreateVulkanInstance() {
        if (!CheckValidationLayerSupport()) {
            throw runtime_error("Validation Layers Not Supported");
        }

        VkApplicationInfo applicationInfo{};
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pApplicationName = "KTX-Compressor";
        applicationInfo.applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
        applicationInfo.pEngineName = "No Engine";
        applicationInfo.engineVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
        applicationInfo.apiVersion = VK_API_VERSION_1_3;


        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &applicationInfo;

        auto requiredExtensions = GetRequiredExtensions();

        createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        if (RendererConstants::ENABLE_VALIDATION_LAYERS) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            auto debugCreateInfo = KTXCompressor::Debugger::GetDebugMessengerCreateInfo();
            createInfo.pNext = &debugCreateInfo;
        } else {
            createInfo.enabledLayerCount = 0;
        }

        VkInstance vkInstance;
        VkResult createInstanceResult = vkCreateInstance(&createInfo, nullptr, &vkInstance);
        if (createInstanceResult != VK_SUCCESS) {
            throw runtime_error("Failed To Create vkInstance");
        }

        cout << "Successfully Created Instance" << endl;

        return vkInstance;
    }

    bool Instance::CheckValidationLayerSupport() {
        if (!RendererConstants::ENABLE_VALIDATION_LAYERS) {
            return true;
        }

        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char *layerName: validationLayers) {
            bool layerFound = false;
            for (const auto &layerProperties: availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    vector<const char *> Instance::GetRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        vector<const char *> requiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (RendererConstants::ENABLE_VALIDATION_LAYERS) {
            requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        if (!RequiredExtensionsMet(requiredExtensions.data(), requiredExtensions.size())) {
            throw runtime_error("Required Extensions Not Met");
        }

        return requiredExtensions;
    }

    bool Instance::RequiredExtensionsMet(const char **requiredExtensions, size_t requiredExtensionsCount) {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        set<string> availableExtensions;
        for (const auto &extension: extensions) {
            availableExtensions.insert(extension.extensionName);
        }

        for (size_t i = 0; i < requiredExtensionsCount; i++) {
            if (!availableExtensions.contains(requiredExtensions[i])) {
                cout << "Missing " << requiredExtensions[i] << endl;
                return false;
            }
        }

        return true;
    }

    // #endregion

    // #region Constructors

    Instance::Instance() {
        vulkanInstance = CreateVulkanInstance();

        if (RendererConstants::ENABLE_VALIDATION_LAYERS) {
            debugger = new Debugger(vulkanInstance);
        }
    }

    // #endregion 

    // #region Destructors

    Instance::~Instance() {
        cout << "Destroy Instance " << endl;

        if (RendererConstants::ENABLE_VALIDATION_LAYERS) {
            delete debugger;
        }
        vkDestroyInstance(vulkanInstance, nullptr);
    }

    // #endregion

} // KTXCompressor