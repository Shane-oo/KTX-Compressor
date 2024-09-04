//
// Created by ShaneMonck on 10/08/2024.
//

#include "BufferUtil.h"
#include "../Commands/SingleTimeCommand.h"

namespace KTXCompressor {

    // #region Private Methods

    void BufferUtil::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        auto singleTimeCommand = new SingleTimeCommand(logicalDevice);
        singleTimeCommand->Copy(srcBuffer, dstBuffer, size);
        delete singleTimeCommand;
    }

    void BufferUtil::CopyImage(VkImage image,
                               VkFormat format,
                               VkBuffer stagingBuffer,
                               uint32_t width,
                               uint32_t height) {
        auto singleTimeCommand = new SingleTimeCommand(logicalDevice);
        singleTimeCommand->CopyImage(image, format, stagingBuffer, width, height);
        delete singleTimeCommand;
    }

    void BufferUtil::TransitionImage(VkImage image,
                                     VkFormat format,
                                     VkImageLayout oldLayout,
                                     VkImageLayout newLayout) {
        auto copyBuffer = new SingleTimeCommand(logicalDevice);
        copyBuffer->TransitionImage(image, format, oldLayout, newLayout);
        delete copyBuffer;
    }

    void BufferUtil::AllocateMemory(VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceMemory &bufferMemory,
                                    const VkMemoryRequirements &memoryRequirements) {
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
    }


    void BufferUtil::CreateStagingBuffer(const void *data,
                                         VkDeviceSize size,
                                         VkBuffer &stagingBuffer,
                                         VkDeviceMemory &stagingBufferMemory) {
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
    }

    void BufferUtil::CleanUpBuffer(VkBuffer stagingBuffer, VkDeviceMemory stagingBufferMemory) {
        vkDestroyBuffer(logicalDevice->GetVulkanDevice(), stagingBuffer, nullptr);
        vkFreeMemory(logicalDevice->GetVulkanDevice(), stagingBufferMemory, nullptr);
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

        AllocateMemory(memoryPropertyFlags, bufferMemory, memoryRequirements);

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
        CreateStagingBuffer(data, size, stagingBuffer, stagingBufferMemory);

        // Create target buffer
        CreateBuffer(size,
                     bufferUsageFlags | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                     memoryPropertyFlags,
                     buffer,
                     bufferMemory);

        // CopyBuffers data from staging buffer to the target buffer
        CopyBuffer(stagingBuffer, buffer, size);

        // Cleanup staging buffer
        CleanUpBuffer(stagingBuffer, stagingBufferMemory);

        cout << "Successfully Created And Filled Buffer" << endl;
    }

    void BufferUtil::CreateAndFillImage(const void *data,
                                        VkDeviceSize size,
                                        VkImageCreateInfo imageCreateInfo,
                                        VkImage &image,
                                        VkDeviceMemory &imageMemory,
                                        VkFormat imageFormat,
                                        uint32_t width,
                                        uint32_t height) {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        CreateStagingBuffer(data, size, stagingBuffer, stagingBufferMemory);

        CreateImage(imageCreateInfo, image, imageMemory, imageFormat);

        CopyImage(image, imageFormat, stagingBuffer, width, height);

        // Cleanup staging buffer
        CleanUpBuffer(stagingBuffer, stagingBufferMemory);

        cout << "Successfully Created And Filled Image" << endl;
    }

    void BufferUtil::CreateImage(VkImageCreateInfo imageCreateInfo,
                                 VkImage &image,
                                 VkDeviceMemory &imageMemory,
                                 VkFormat format,
                                 bool transition,
                                 VkImageLayout oldLayout,
                                 VkImageLayout newLayout,
                                 VkMemoryPropertyFlags memoryPropertyFlags) {
        VkResult createImageResult = vkCreateImage(logicalDevice->GetVulkanDevice(), &imageCreateInfo, nullptr,
                                                   &image);
        if (createImageResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Image");
        }

        cout << "Successfully Created Vulkan Image" << endl;

        VkMemoryRequirements memoryRequirements;
        vkGetImageMemoryRequirements(logicalDevice->GetVulkanDevice(), image, &memoryRequirements);

        VkMemoryAllocateInfo memoryAllocateInfo = {};
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

        AllocateMemory(memoryPropertyFlags, imageMemory, memoryRequirements);

        VkResult bindImageResult = vkBindImageMemory(logicalDevice->GetVulkanDevice(), image, imageMemory, 0);
        if (bindImageResult != VK_SUCCESS) {
            throw runtime_error("Failed to Bind Image to Image Memory");
        }

        cout << "Successfully Bound Image To Image Memory" << endl;

        if (transition) {
            cout << "Transitioning New Image from " << oldLayout << " To " << newLayout << endl;

            TransitionImage(image, format, oldLayout, newLayout);
        }
    }



    // #endregion

} // KTXCompressor