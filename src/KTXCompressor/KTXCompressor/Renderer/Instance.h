//
// Created by shane on 6/07/2024.
//

#ifndef KTXCOMPRESSOR_INSTANCE_H
#define KTXCOMPRESSOR_INSTANCE_H

#include "../Common.h"
#include <vulkan/vulkan_core.h>

namespace KTXCompressor {

    class Instance {
    public:
        Instance();

        ~Instance();

    private:
        VkInstance instance;

    private:
        void CreateInstance();

        bool CheckValidationLayerSupport();

        vector<const char *> GetRequiredExtensions();

        static bool RequiredExtensionsMet(const char **requiredExtensions, uint32_t requiredExtensionsCount);
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_INSTANCE_H
