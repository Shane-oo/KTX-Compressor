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
        Debugger();

        ~Debugger();

    public:


    private:
        VkDebugUtilsMessengerEXT debugMessenger;

    public:
        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                            VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                            void *pUserData);

        VkDebugUtilsMessengerCreateInfoEXT GetDebugMessengerCreateInfo();

        void CreateDebugMessengerForVkInstance(VkInstance vkInstance);

        void DestroyDebugMessengerForVkInstance(VkInstance vkInstance);
    };


} // KTXCompressor

#endif //KTXCOMPRESSOR_DEBUGGER_H
