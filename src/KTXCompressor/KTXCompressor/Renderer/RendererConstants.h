//
// Created by shane on 7/07/2024.
//

#ifndef KTXCOMPRESSOR_RENDERERCONSTANTS_H
#define KTXCOMPRESSOR_RENDERERCONSTANTS_H

namespace KTXCompressor {

    class RendererConstants {
    public:
#ifdef NDEBUG
        static const bool enableValidationLayers = false;
#else
        static const bool enableValidationLayers = true;
#endif
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_RENDERERCONSTANTS_H
