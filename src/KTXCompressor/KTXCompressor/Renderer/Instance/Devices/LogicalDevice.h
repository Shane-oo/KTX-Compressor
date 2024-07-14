//
// Created by shane on 14/07/2024.
//

#ifndef KTXCOMPRESSOR_LOGICALDEVICE_H
#define KTXCOMPRESSOR_LOGICALDEVICE_H

#include "../../../Common.h"
#include "PhysicalDevice.h"
#include "Queues/GraphicsQueue.h"
#include <vulkan/vulkan_core.h>

namespace KTXCompressor {

    class LogicalDevice {
    public:
        LogicalDevice(PhysicalDevice *physicalDevice);

        ~LogicalDevice();

    private:
        VkDevice vulkanDevice;
        GraphicsQueue *graphicsQueue;

    private :
        VkDevice CreateLogicalDevice(PhysicalDevice *physicalDevice);

        VkQueue RetrieveQueue(uint32_t queueFamilyIndex);
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_LOGICALDEVICE_H
