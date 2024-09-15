//
// Created by ShaneMonck on 3/09/2024.
//

#ifndef KTX_COMPRESSOR_IMAGETEXTURE_H
#define KTX_COMPRESSOR_IMAGETEXTURE_H

#include "Texture.h"

#include <OpenImageIO/imageio.h>

using namespace OIIO;

namespace KTXCompressor {

    class ImageTexture : public Texture {
    public:
        ImageTexture(const string &fileName, LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice);

        ~ImageTexture();
    private:
        string name;
        unique_ptr<ImageInput> ImageInput;
        VkSampler sampler;

        void LoadImageForFile(const string &fileName);

        static void AddAlphaChannelToImage(unique_ptr<unsigned char[]> &pixels,
                                           uint32_t width,
                                           uint32_t height,
                                           int channels);

    public:
        VkSampler GetSampler() override{
            return sampler;
        }
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_IMAGETEXTURE_H
