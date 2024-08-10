//
// Created by ShaneMonck on 10/08/2024.
//

#ifndef KTX_COMPRESSOR_BUFFERUTIL_H
#define KTX_COMPRESSOR_BUFFERUTIL_H

#include <vulkan/vulkan_core.h>
#include "../../Common.h"
#include "../Instance/Devices/LogicalDevice.h"

namespace KTXCompressor {

    class BufferUtil {
    public:
        BufferUtil(LogicalDevice *logicalDevice, PhysicalDevice* physicalDevice);

        ~BufferUtil();

        void CreateAndFillBuffer(const void* data,
                                 VkDeviceSize size,
                                 VkBufferUsageFlags bufferUsageFlags,
                                 VkMemoryPropertyFlags memoryPropertyFlags,
                                 VkBuffer& buffer,
                                 VkDeviceMemory& bufferMemory);

    private:
        LogicalDevice *logicalDevice;
        PhysicalDevice *physicalDevice;

        void CreateBuffer(VkDeviceSize deviceSize,
                          VkBufferUsageFlags bufferUsageFlags,
                          VkMemoryPropertyFlags memoryPropertyFlags,
                          VkBuffer &buffer,
                          VkDeviceMemory &bufferMemory);

        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_BUFFERUTIL_H
