//
// Created by shane on 7/07/2024.
//

#ifndef KTXCOMPRESSOR_RENDERERCONSTANTS_H
#define KTXCOMPRESSOR_RENDERERCONSTANTS_H

#include "../Common.h"

namespace KTXCompressor {

    class RendererConstants {
    public:
#ifdef NDEBUG
        static const bool enableValidationLayers = false;
#else
        static const bool enableValidationLayers = true;
#endif
        static const vector<const char *> requiredDeviceExtensions;
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_RENDERERCONSTANTS_H
