//
// Created by ShaneMonck on 10/08/2024.
//

#include "BufferUtil.h"
#include "../Instance/Devices/PhysicalDevice.h"

namespace KTXCompressor {
    // #region Constructors

    BufferUtil::BufferUtil(VkDevice vulkanDevice, PhysicalDevice *physicalDevice) {
        this->vulkanDevice = vulkanDevice;
        this->physicalDevice = physicalDevice;

    }

    // #endregion

    // #region Destructors

    BufferUtil::~BufferUtil() {

    }

    // #endregion


    // #region Public Methods

    void BufferUtil::CreateBuffer(VkDeviceSize deviceSize,
                                  VkBufferUsageFlags bufferUsageFlags,
                                  VkMemoryPropertyFlags memoryPropertyFlags,
                                  VkBuffer &buffer,
                                  VkDeviceMemory &bufferMemory) {
        VkBufferCreateInfo bufferCreateInfo = {};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size = deviceSize;
        bufferCreateInfo.usage = bufferUsageFlags;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VkResult createBufferResult = vkCreateBuffer(vulkanDevice, &bufferCreateInfo, nullptr, &buffer);
        if (createBufferResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Buffer");
        }

        cout << "Successfully Created Buffer, size: " << to_string(deviceSize) << endl;

        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(vulkanDevice, buffer, &memoryRequirements);

        VkMemoryAllocateInfo memoryAllocateInfo = {};
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = physicalDevice->FindMemoryType(memoryRequirements.memoryTypeBits,
                                                                            memoryPropertyFlags);

        VkResult allocateMemoryResult = vkAllocateMemory(vulkanDevice, &memoryAllocateInfo, nullptr, &bufferMemory);
        if (allocateMemoryResult != VK_SUCCESS) {
            throw runtime_error("Failed to Allocate Buffer Memory");
        }

        cout << "Successfully Allocated Buffer Memory, size: " << to_string(memoryRequirements.size) << endl;

        VkResult bindBufferResult = vkBindBufferMemory(vulkanDevice, buffer, bufferMemory, 0);
        if (bindBufferResult != VK_SUCCESS) {
            throw runtime_error("Failed to Bind Buffer to Buffer Memory");
        }

        cout << "Successfully Bound Buffer To Buffer Memory" << endl;
    }

    void BufferUtil::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
         //todo use the CopyBufferCommand...
    }

    // #endregion

} // KTXCompressor