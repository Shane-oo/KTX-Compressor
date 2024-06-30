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

        ~Texture();

    private:
        static void OpenImage(const string &fileName,
                              unique_ptr<ImageInput> &imageInput,
                              ImageSpec &resultantImageSpec);

        std::unique_ptr<unsigned char[]> LoadInputImage(ImageInput &inputImage, ImageSpec imageSpec);

        void WriteNewKtx2Image(const string& newFileName);

        void CreateKtxTexture(ImageInput &inputImage, const ImageSpec &imageSpec);

        void CompressTexture() const;

        unique_ptr<unsigned char[]> GetCompressedPixelsFromKtxTexture(ktx_size_t &writtenSize) const;

    public:
        ktxTexture2 *myKtxTexture = nullptr;

    };


} // KTXCompressor

#endif //KTXCOMPRESSOR_TEXTURE_H
