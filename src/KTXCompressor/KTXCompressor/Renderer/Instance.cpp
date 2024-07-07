//
// Created by shane on 6/07/2024.
//

#include <GLFW/glfw3.h>
#include "Instance.h"
#include "RendererConstants.h"

namespace KTXCompressor {

    // #region Constants

    static const vector<const char *> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };


    // #endregion

    // #region Private Methods

    void Instance::CreateInstance() {
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

        if (RendererConstants::enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        VkResult createInstanceResult = vkCreateInstance(&createInfo, nullptr, &instance);
        if (createInstanceResult != VK_SUCCESS) {
            throw runtime_error("Failed To Create instance");
        }
        cout << "Successfully Created Instance" << endl;
    }

    bool Instance::CheckValidationLayerSupport() {
        if (!RendererConstants::enableValidationLayers) {
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

        if (RendererConstants::enableValidationLayers) {
            requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        if (!RequiredExtensionsMet(requiredExtensions.data(), requiredExtensions.size())) {
            throw runtime_error("Required Extensions Not Met");
        }

        return requiredExtensions;
    }

    bool Instance::RequiredExtensionsMet(const char **requiredExtensions, uint32_t requiredExtensionsCount) {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        set<string> availableExtensions;
        for (const auto &extension: extensions) {
            availableExtensions.insert(extension.extensionName);
        }

        for (int i = 0; i < requiredExtensionsCount; i++) {
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
        CreateInstance();
    }

    // #endregion 

    // #region Destructors

    Instance::~Instance() {
        vkDestroyInstance(instance, nullptr);
    }

    // #endregion

} // KTXCompressor