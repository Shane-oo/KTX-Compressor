//
// Created by shane on 14/07/2024.
//

#include "LogicalDevice.h"
#include "Queues/QueueFamily.h"
#include "../../RendererConstants.h"


namespace KTXCompressor {
    // #region Private Methods

    VkDevice LogicalDevice::CreateLogicalDevice(PhysicalDevice *physicalDevice) {

        QueueFamilyIndices indices = physicalDevice->GetQueueFamilyIndices();

        // number of queues we want for a single queue family
        VkDeviceQueueCreateInfo deviceQueueCreateInfo{};
        deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.queueFamilyIndex = indices.graphicsFamilyIndex.value();
        deviceQueueCreateInfo.queueCount = 1;

        // vulkan lets you assign prioritizes to queues to influence scheduling of command buffer execution
        // this is required even if there is only a single queue
        float queuePriority = 1.0f;
        deviceQueueCreateInfo.pQueuePriorities = &queuePriority;


        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
        createInfo.queueCreateInfoCount = 1;

        // Specify used device features  (TODO could move this to the Physical Device class itself...)
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

        return queue;
    }

    // #endregion

    // #region Constructors

    LogicalDevice::LogicalDevice(PhysicalDevice *physicalDevice) {
        vulkanDevice = CreateLogicalDevice(physicalDevice);

        uint32_t graphicsFamilyIndex = physicalDevice->GetQueueFamilyIndices().graphicsFamilyIndex.value();
        graphicsQueue = new GraphicsQueue(RetrieveQueue(graphicsFamilyIndex));
    }

    // #endregion

    // #region Destructors

    LogicalDevice::~LogicalDevice() {
        vkDestroyDevice(vulkanDevice, nullptr);

        delete graphicsQueue;
    }



    // #endregion

} // KTXCompressor