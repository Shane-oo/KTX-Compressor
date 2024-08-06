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
        static const bool ENABLE_VALIDATION_LAYERS = false;
#else
        static const bool ENABLE_VALIDATION_LAYERS = true;
#endif
        static const vector<const char *> REQUIRED_DEVICE_EXTENSION;

        static const int MAX_FRAMES_IN_FLIGHT = 2;
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_RENDERERCONSTANTS_H
