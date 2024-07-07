//
// Created by shane on 7/07/2024.
//

#include "Debugger.h"
#include "../RendererConstants.h"


namespace KTXCompressor {

    // #region Private Static Methods

    VkBool32 Debugger::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                     VkDebugUtilsMessageTypeFlagsEXT messageType,
                                     const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {

        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            // Message is important enough to show
            switch (messageType) {
                case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
                    /*std::cout << "Some event has happened that is unrelated to the specification or performance"
                     * << std::endl;
                    */
                    return VK_FALSE;
                case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
                    std::cout
                            << "Something has happened that violates the specification or indicates a possible mistake"
                            << std::endl;
                    break;
                case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
                    std::cout << "Potential non-optimal use of Vulkan" << std::endl;
                    break;
                case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
                    std::cout
                            << "The implementation has modified the set of GPU-visible virtual addresses associated with a Vulkan object."
                            << std::endl;
                    break;
                default:
                    std::cout << "Something unexpected has happened. Message Type: " << messageType << std::endl;
                    break;
            }

            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        }

        return VK_FALSE;
    }

    // #endregion


    // #region Constructors

    Debugger::Debugger() {
    }

    // #endregion

    // #region Destructors

    Debugger::~Debugger() {

    }

    // #endregion

    // #region Public Methods

    VkDebugUtilsMessengerCreateInfoEXT Debugger::GetDebugMessengerCreateInfo() {
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                     | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                     | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                 | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                 | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = DebugCallback;
        createInfo.pUserData = nullptr; // Optional

        return createInfo;
    }

    void Debugger::CreateDebugMessengerForVkInstance(VkInstance vkInstance) {
        auto createInfo = GetDebugMessengerCreateInfo();

        auto createFunc = (PFN_vkCreateDebugUtilsMessengerEXT)
                vkGetInstanceProcAddr(vkInstance,
                                      "vkCreateDebugUtilsMessengerEXT");
        if (createFunc != nullptr) {
            VkResult createMessengerResult = createFunc(vkInstance,
                                                        &createInfo,
                                                        nullptr,
                                                        &debugMessenger);
            if (createMessengerResult == VK_SUCCESS) {
                cout << "Successfully created Debug Messenger " << endl;

                return;
            }
        }

        throw runtime_error("Failed to Create Debug Messenger");
    }

    void Debugger::DestroyDebugMessengerForVkInstance(VkInstance vkInstance) {
        auto destroyFunc = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vkInstance,
                                                                                       "vkDestroyDebugUtilsMessengerEXT");
        if (destroyFunc != nullptr) {
            destroyFunc(vkInstance, debugMessenger, nullptr);
        }

    }


    // #endregion

} // KTXCompressor