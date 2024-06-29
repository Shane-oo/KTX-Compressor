//
// Created by shane on 29/06/2024.
//

#ifndef KTXCOMPRESSOR_TEXTURE_H
#define KTXCOMPRESSOR_TEXTURE_H

#include <ktx.h>
#include <vulkan/vulkan_core.h>

#include <OpenImageIO/imageio.h>

using namespace std;
using namespace OIIO;

namespace KTXCompressor {

    class Texture {
        
    public:
        Texture(const string &fileName);
        
    private:
        void CreateKtxTexture(const ImageSpec& imageSpec);
    public:
        ktxTexture2* ktxTexture = nullptr;

        /*[[nodiscard]] ktxTexture2 **GetKtxTexture() const {
            return ktxTexture;
        }*/
    };


} // KTXCompressor

#endif //KTXCOMPRESSOR_TEXTURE_H
