//
// Created by shane on 14/07/2024.
//

#include "LogicalDevice.h"
#include "Queues/QueueFamily.h"
#include "../../RendererConstants.h"


namespace KTXCompressor {
    // #region Private Methods

    VkDevice LogicalDevice::CreateLogicalVulkanDevice(PhysicalDevice *physicalDevice) {

        vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
        set<uint32_t> uniqueQueueFamilies = {
                physicalDevice->GetGraphicsFamilyIndex(),
                physicalDevice->GetPresentFamilyIndex()
        };
        // vulkan lets you assign prioritizes to queues to influence scheduling of command buffer execution
        // this is required even if there is only a single queue
        float queuePriority = 1.0f;
        for (uint32_t queueFamilyIndex: uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo deviceQueueCreateInfo{};
            deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex;
            deviceQueueCreateInfo.queueCount = 1;
            deviceQueueCreateInfo.pQueuePriorities = &queuePriority;

            deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
        }

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(deviceQueueCreateInfos.size());
        createInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();

        // Specify used device features 
        VkPhysicalDeviceFeatures enabledDeviceFeatures = physicalDevice->GetRequiredPhysicalDeviceFeatures();
        createInfo.pEnabledFeatures = &enabledDeviceFeatures;

        VkDevice logicalDevice;
        VkResult createLogicalDeviceResult = vkCreateDevice(physicalDevice->GetVulkanPhysicalDevice(),
                                                            &createInfo,
                                                            nullptr,
                                                            &logicalDevice);
        if (createLogicalDeviceResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Logical Device");
        }

        cout << "Successfully created Logical Device" << endl;

        return logicalDevice;
    }

    VkQueue LogicalDevice::RetrieveQueue(uint32_t queueFamilyIndex) {
        VkQueue queue;

        vkGetDeviceQueue(vulkanDevice,
                         queueFamilyIndex,
                         0,
                         &queue);

        if (!queue) {
            throw runtime_error("Failed to Retrieve Queue with queueFamilyIndex: " + to_string(queueFamilyIndex));
        }

        return queue;
    }

    // #endregion

    // #region Constructors

    LogicalDevice::LogicalDevice(PhysicalDevice *physicalDevice) {
        vulkanDevice = CreateLogicalVulkanDevice(physicalDevice);

        uint32_t graphicsFamilyIndex = physicalDevice->GetGraphicsFamilyIndex();
        graphicsQueue = new GraphicsQueue(RetrieveQueue(graphicsFamilyIndex));

        uint32_t presentFamilyIndex = physicalDevice->GetPresentFamilyIndex();
        presentQueue = new PresentQueue(RetrieveQueue(presentFamilyIndex));
    }

    // #endregion

    // #region Destructors

    LogicalDevice::~LogicalDevice() {
        cout << "Destroy Logical Device " << endl;

        vkDestroyDevice(vulkanDevice, nullptr);

        delete graphicsQueue;
        delete presentQueue;
    }



    // #endregion

} // KTXCompressor