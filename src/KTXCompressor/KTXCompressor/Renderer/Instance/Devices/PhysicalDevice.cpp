//
// Created by shane on 7/07/2024.
//

#include "PhysicalDevice.h"
#include "Queues/QueueFamily.h"
#include "../../RendererConstants.h"
#include "../../Presentation/SwapChain.h"

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

                vkGetPhysicalDeviceMemoryProperties(device, &physicalDeviceMemoryProperties);

                return device;
            }
        }

        throw runtime_error("Failed to find a suitable GPU");
    }

    bool PhysicalDevice::IsDeviceSuitable(VkPhysicalDevice device) {
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        bool suitable = false;
        if (CheckDeviceExtensionsSupported(device)) {
            bool queueFamilyIndicesComplete = queueFamily->FindQueueFamiliesForPhysicalDevice(device).IsComplete();

            bool swapChainAdequate = SwapChain::QuerySwapChainSupport(device,
                                                                      queueFamily->GetVulkanSurface())
                    .IsAdequate();

            VkPhysicalDeviceFeatures supportedFeatures;
            vkGetPhysicalDeviceFeatures(device, &supportedFeatures);


            suitable = queueFamilyIndicesComplete && swapChainAdequate && supportedFeatures.samplerAnisotropy;
            if (suitable) {
                vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);

                cout << "Device " << physicalDeviceProperties.deviceName << " Is Suitable" << endl;
            }

        }

        return suitable;
    }

    bool PhysicalDevice::CheckDeviceExtensionsSupported(VkPhysicalDevice device) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        set<string> requiredExtensions(RendererConstants::REQUIRED_DEVICE_EXTENSION.begin(),
                                       RendererConstants::REQUIRED_DEVICE_EXTENSION.end());

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

    // #region Public Methods

    uint32_t PhysicalDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags memoryPropertyFlags) {

        for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++) {

            if ((typeFilter & (1 << i))
                && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags
                    & memoryPropertyFlags) == memoryPropertyFlags) {
                return i;
            }
        }

        throw runtime_error("Failed to Find Suitable Memory Type");
    }

    // #endregion 

} // KTXCompressor