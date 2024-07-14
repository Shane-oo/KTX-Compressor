//
// Created by shane on 6/07/2024.
//

#ifndef KTXCOMPRESSOR_INSTANCE_H
#define KTXCOMPRESSOR_INSTANCE_H

#include "../../Common.h"
#include "Debug/Debugger.h"
#include "Devices/PhysicalDevice.h"
#include <vulkan/vulkan_core.h>

namespace KTXCompressor {

    class Instance {
    public:
        Instance();

        ~Instance();

    private:
        VkInstance vulkanInstance;
        Debugger *debugger;
        PhysicalDevice *physicalDevice;

    private:
        VkInstance CreateVulkanInstance();

        bool CheckValidationLayerSupport();

        vector<const char *> GetRequiredExtensions();

        static bool RequiredExtensionsMet(const char **requiredExtensions, size_t requiredExtensionsCount);

    public:
/*        [[nodiscard]] VkInstance GetVkInstance() {
            return vkInstance;
        }*/
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_INSTANCE_H
