//
// Created by ShaneMonck on 10/08/2024.
//

#ifndef KTX_COMPRESSOR_BUFFERUTIL_H
#define KTX_COMPRESSOR_BUFFERUTIL_H

#include <vulkan/vulkan_core.h>
#include "../../Common.h"
#include "../Instance/Devices/LogicalDevice.h"

namespace KTXCompressor {
    /*
    * Future Work:
    * Try to experiment with this by creating a setupCommandBuffer that the helper functions record 
    * commands into, and add a flushSetupCommands to execute the commands that 
    * have been recorded so far.
    */

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
                                VkFormat imageFormat,
                                uint32_t width,
                                uint32_t height,
                                VkMemoryPropertyFlags memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        void CreateImage(VkImageCreateInfo imageCreateInfo,
                         VkImage &image,
                         VkDeviceMemory &imageMemory,
                         VkMemoryPropertyFlags memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        void CreateBuffer(VkDeviceSize deviceSize,
                          VkBufferUsageFlags bufferUsageFlags,
                          VkMemoryPropertyFlags memoryPropertyFlags,
                          VkBuffer &buffer,
                          VkDeviceMemory &bufferMemory);

    private:
        LogicalDevice *logicalDevice;
        PhysicalDevice *physicalDevice;

        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        void CopyImage(VkImage image,
                       VkFormat format,
                       VkBuffer stagingBuffer,
                       uint32_t width,
                       uint32_t height);

        void
        CreateStagingBuffer(const void *data, VkDeviceSize size, VkBuffer &stagingBuffer,
                            VkDeviceMemory &stagingBufferMemory);

     

        void AllocateMemory(VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceMemory &bufferMemory,
                            const VkMemoryRequirements &memoryRequirements);

        void CleanUpBuffer(VkBuffer stagingBuffer, VkDeviceMemory stagingBufferMemory);
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_BUFFERUTIL_H
