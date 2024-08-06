//
// Created by shane on 14/07/2024.
//

#include <vulkan/vulkan_core.h>
#include "RendererConstants.h"

namespace KTXCompressor {

    // #region Constants
    const vector<const char *> RendererConstants::REQUIRED_DEVICE_EXTENSION = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
            VK_EXT_GRAPHICS_PIPELINE_LIBRARY_EXTENSION_NAME
    };

    // #endregion

} // KTXCompressor