//
// Created by ShaneMonck on 10/08/2024.
//

#ifndef KTX_COMPRESSOR_BUFFERUTIL_H
#define KTX_COMPRESSOR_BUFFERUTIL_H

#include <vulkan/vulkan_core.h>
#include "../../Common.h"
#include "../Instance/Devices/PhysicalDevice.h"

namespace KTXCompressor {

    class BufferUtil {
    public:
        BufferUtil(VkDevice vulkanDevice, PhysicalDevice *physicalDevice);

        ~BufferUtil();

        void CreateBuffer(VkDeviceSize deviceSize,
                          VkBufferUsageFlags bufferUsageFlags,
                          VkMemoryPropertyFlags memoryPropertyFlags,
                          VkBuffer &buffer,
                          VkDeviceMemory &bufferMemory);

        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    private:
        VkDevice vulkanDevice;
        PhysicalDevice *physicalDevice;
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_BUFFERUTIL_H
