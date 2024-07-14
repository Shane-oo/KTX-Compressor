//
// Created by shane on 7/07/2024.
//

#include "PhysicalDevice.h"
#include "Queues/QueueFamily.h"

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
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        bool QueueFamilyIndicesComplete = queueFamily->FindQueueFamiliesForPhysicalDevice(device)
                .IsComplete();

        cout << "Device " << deviceProperties.deviceName << " Is Suitable" << endl;

        return QueueFamilyIndicesComplete;
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