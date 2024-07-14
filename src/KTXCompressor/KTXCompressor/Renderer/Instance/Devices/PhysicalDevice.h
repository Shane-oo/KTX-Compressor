//
// Created by shane on 7/07/2024.
//

#ifndef KTXCOMPRESSOR_PHYSICALDEVICE_H
#define KTXCOMPRESSOR_PHYSICALDEVICE_H

#include "../../../Common.h"
#include <vulkan/vulkan_core.h>

namespace KTXCompressor {

    class PhysicalDevice {

    public:
        PhysicalDevice(VkInstance vulkanInstance);

        ~PhysicalDevice();

    private:
        VkPhysicalDevice vulkanPhysicalDevice;

    private:
        VkPhysicalDevice PickPhysicalDevice(VkInstance vulkanInstance);
        
        bool IsDeviceSuitable(VkPhysicalDevice device);
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_PHYSICALDEVICE_H
