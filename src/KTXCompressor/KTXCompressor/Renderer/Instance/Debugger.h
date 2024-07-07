//
// Created by shane on 7/07/2024.
//

#ifndef KTXCOMPRESSOR_DEBUGGER_H
#define KTXCOMPRESSOR_DEBUGGER_H


#include "../../Common.h"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace KTXCompressor {

    class Debugger {
    public:
        Debugger(VkInstance vkInstance);

        ~Debugger();

    public:
        static VkDebugUtilsMessengerCreateInfoEXT GetDebugMessengerCreateInfo();

    private:
        VkDebugUtilsMessengerEXT debugMessenger;
        VkInstance vulkanInstance;

    private:
        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                            VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                            void *pUserData);

        VkDebugUtilsMessengerEXT CreateDebugMessenger();

        void DestroyDebugMessenger();
    };


} // KTXCompressor

#endif //KTXCOMPRESSOR_DEBUGGER_H
