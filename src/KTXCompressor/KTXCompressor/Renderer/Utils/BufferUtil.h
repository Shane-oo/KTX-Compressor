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
        BufferUtil(LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice);

        ~BufferUtil();

        void CreateAndFillBuffer(const void *data,
                                 VkDeviceSize size,
                                 VkBufferUsageFlags bufferUsageFlags,
                                 VkMemoryPropertyFlags memoryPropertyFlags,
                                 VkBuffer &buffer,
                                 VkDeviceMemory &bufferMemory);

        void CreateAndFillImage(const void *data,
                                VkDeviceSize size,
                                VkImageCreateInfo imageCreateInfo,
                                VkImage &image,
                                VkDeviceMemory &imageMemory,
                                VkMemoryPropertyFlags memoryPropertyFlags);

        void CreateBuffer(VkDeviceSize deviceSize,
                          VkBufferUsageFlags bufferUsageFlags,
                          VkMemoryPropertyFlags memoryPropertyFlags,
                          VkBuffer &buffer,
                          VkDeviceMemory &bufferMemory);

    private:
        LogicalDevice *logicalDevice;
        PhysicalDevice *physicalDevice;

        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        void
        CreateStagingBuffer(const void *data, VkDeviceSize size, VkBuffer &stagingBuffer,
                            VkDeviceMemory &stagingBufferMemory);

        void CreateImage(VkImageCreateInfo imageCreateInfo,
                         VkImage &image,
                         VkDeviceMemory &imageMemory,
                         VkMemoryPropertyFlags memoryPropertyFlags);

        void AllocateMemory(VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceMemory &bufferMemory,
                                      const VkMemoryRequirements &memoryRequirements);
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_BUFFERUTIL_H
