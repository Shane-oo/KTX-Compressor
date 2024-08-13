//
// Created by ShaneMonck on 10/08/2024.
//

#include "BufferUtil.h"
#include "../Commands/CopyBufferCommand.h"

namespace KTXCompressor {
    // #region Private Methods




    void BufferUtil::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        auto copyBuffer = new CopyBufferCommand(logicalDevice, srcBuffer, dstBuffer, size);
        delete copyBuffer;
    }

    // #endregion


    // #region Constructors

    BufferUtil::BufferUtil(LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice) {
        this->logicalDevice = logicalDevice;
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

        VkResult createBufferResult = vkCreateBuffer(logicalDevice->GetVulkanDevice(), &bufferCreateInfo, nullptr,
                                                     &buffer);
        if (createBufferResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Buffer");
        }

        cout << "Successfully Created Buffer, size: " << to_string(deviceSize) << endl;

        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(logicalDevice->GetVulkanDevice(), buffer, &memoryRequirements);

        VkMemoryAllocateInfo memoryAllocateInfo = {};
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = physicalDevice->FindMemoryType(memoryRequirements.memoryTypeBits,
                                                                            memoryPropertyFlags);

        VkResult allocateMemoryResult = vkAllocateMemory(logicalDevice->GetVulkanDevice(), &memoryAllocateInfo, nullptr,
                                                         &bufferMemory);
        if (allocateMemoryResult != VK_SUCCESS) {
            throw runtime_error("Failed to Allocate Buffer Memory");
        }

        cout << "Successfully Allocated Buffer Memory, size: " << to_string(memoryRequirements.size) << endl;

        VkResult bindBufferResult = vkBindBufferMemory(logicalDevice->GetVulkanDevice(), buffer, bufferMemory, 0);
        if (bindBufferResult != VK_SUCCESS) {
            throw runtime_error("Failed to Bind Buffer to Buffer Memory");
        }

        cout << "Successfully Bound Buffer To Buffer Memory" << endl;
    }

    void BufferUtil::CreateAndFillBuffer(const void *data,
                                         VkDeviceSize size,
                                         VkBufferUsageFlags bufferUsageFlags,
                                         VkMemoryPropertyFlags memoryPropertyFlags,
                                         VkBuffer &buffer,
                                         VkDeviceMemory &bufferMemory) {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        // Create staging buffer
        CreateBuffer(size,
                     VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     stagingBuffer,
                     stagingBufferMemory);

        // Fill the staging buffer with data
        void *mappedData;
        vkMapMemory(logicalDevice->GetVulkanDevice(), stagingBufferMemory, 0, size, 0, &mappedData);
        memcpy(mappedData, data, static_cast<size_t>(size));
        vkUnmapMemory(logicalDevice->GetVulkanDevice(), stagingBufferMemory);

        // Create target buffer
        CreateBuffer(size,
                     bufferUsageFlags | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                     memoryPropertyFlags,
                     buffer,
                     bufferMemory);

        // Copy data from staging buffer to the target buffer
        CopyBuffer(stagingBuffer, buffer, size);

        // Cleanup staging buffer
        vkDestroyBuffer(logicalDevice->GetVulkanDevice(), stagingBuffer, nullptr);
        vkFreeMemory(logicalDevice->GetVulkanDevice(), stagingBufferMemory, nullptr);

        cout << "Successfully Created And Filled Buffer" << endl;
    }


    // #endregion

} // KTXCompressor