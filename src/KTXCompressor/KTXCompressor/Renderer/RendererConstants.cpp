//
// Created by shane on 14/07/2024.
//

#include <vulkan/vulkan_core.h>
#include "RendererConstants.h"

namespace KTXCompressor {

    // #region Constants
    const vector<const char *> RendererConstants::requiredDeviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    // #endregion

} // KTXCompressor