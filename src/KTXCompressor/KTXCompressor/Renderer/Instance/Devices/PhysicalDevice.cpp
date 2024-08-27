//
// Created by shane on 7/07/2024.
//

#include "PhysicalDevice.h"
#include "Queues/QueueFamily.h"
#include "../../RendererConstants.h"
#include "../../Presentation/SwapChain.h"

namespace KTXCompressor {
    // #region Private Methods

    // Check if the device supports sampling and transfers for the selected image
    bool PhysicalDevice::GetVulkanFormatSupported(VkFormat format) {

        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(vulkanPhysicalDevice, format, &formatProperties);
        return ((formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_TRANSFER_DST_BIT) &&
                (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT));
    }

    void PhysicalDevice::SetAvailableTargetFormatsForKtx() {

        // Block Compression
        if (supportedFeatures.textureCompressionBC) {
            // BC7 is the preferred block compression if available
            if (GetVulkanFormatSupported(VK_FORMAT_BC7_SRGB_BLOCK)) {
                availableTargetFormats.push_back(KTX_TTF_BC7_RGBA);
            }

            if (GetVulkanFormatSupported(VK_FORMAT_BC3_SRGB_BLOCK)) {
                availableTargetFormats.push_back(KTX_TTF_BC3_RGBA);
            }
        }

        // Adaptive scalable texture compression
        if (supportedFeatures.textureCompressionASTC_LDR) {
            if (GetVulkanFormatSupported(VK_FORMAT_ASTC_4x4_SRGB_BLOCK)) {
                availableTargetFormats.push_back(KTX_TTF_ASTC_4x4_RGBA);
            }
        }

        // Ericcson texture Compression
        if (supportedFeatures.textureCompressionETC2) {
            if (GetVulkanFormatSupported(VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK)) {
                availableTargetFormats.push_back(KTX_TTF_ETC2_RGBA);
            }
        }

        // Always add uncompressed RGBA as a valid target
        availableTargetFormats.push_back(KTX_TTF_RGBA32);
    }


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
        SetAvailableTargetFormatsForKtx();
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