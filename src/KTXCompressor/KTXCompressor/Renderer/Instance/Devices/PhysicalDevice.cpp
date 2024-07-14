//
// Created by shane on 7/07/2024.
//

#include "PhysicalDevice.h"
#include "Queues/QueueFamily.h"
#include "../../RendererConstants.h"

namespace KTXCompressor {
    // #region Private Methods

    // Look for and select a graphics card in the system that supports the features we need
    VkPhysicalDevice PhysicalDevice::PickPhysicalDevice(VkInstance vulkanInstance) {

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, nullptr);
        if (deviceCount == 0) {
            throw runtime_error("Failed to find GPUs with Vulkan Support");
        }

        vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, devices.data());

        for (const auto &device: devices) {
            if (IsDeviceSuitable(device)) {

                auto queueFamilyIndices = queueFamily->FindQueueFamiliesForPhysicalDevice(device);
                queueFamily->SetSelectedQueueFamilyIndices(queueFamilyIndices);

                return device;
            }
        }

        throw runtime_error("Failed to find a suitable GPU");
    }

    bool PhysicalDevice::IsDeviceSuitable(VkPhysicalDevice device) {

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        bool queueFamilyIndicesComplete = queueFamily->FindQueueFamiliesForPhysicalDevice(device)
                .IsComplete();

        bool suitable = queueFamilyIndicesComplete && CheckDeviceExtensionsSupported(device);
        if (suitable) {
            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(device, &deviceProperties);

            cout << "Device " << deviceProperties.deviceName << " Is Suitable" << endl;
        }

        return suitable;
    }

    bool PhysicalDevice::CheckDeviceExtensionsSupported(VkPhysicalDevice device) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        set<string> requiredExtensions(RendererConstants::requiredDeviceExtensions.begin(),
                                       RendererConstants::requiredDeviceExtensions.end());

        for (const auto &extension: availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    // #endregion

    // #region Constructors

    PhysicalDevice::PhysicalDevice(VkInstance vulkanInstance, QueueFamily *queueFamily) {
        this->queueFamily = queueFamily;
        vulkanPhysicalDevice = PickPhysicalDevice(vulkanInstance);
    }

    // #endregion

    // #region Destructors

    PhysicalDevice::~PhysicalDevice() {
        cout << "Destroy Physical Device" << endl;
        delete queueFamily;
        // no need to clean up vulkanPhysicalDevice - will be destroyed when vulkanInstance is destroyed
    }



    // #endregion 

} // KTXCompressor