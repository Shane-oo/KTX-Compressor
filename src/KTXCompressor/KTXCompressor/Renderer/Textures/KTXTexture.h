//
// Created by ShaneMonck on 3/09/2024.
//

#ifndef KTX_COMPRESSOR_KTXTEXTURE_H
#define KTX_COMPRESSOR_KTXTEXTURE_H

#include "Texture.h"

namespace KTXCompressor {

    class KTXTexture : public Texture {
    public:
        KTXTexture(const string &fileName, LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice);
        ~KTXTexture();
    private:
        string name;
        VkSampler sampler;

        void LoadKtx2File(const string &fileName);
        
    public:
        VkSampler GetSampler() override {
            return sampler;
        }
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_KTXTEXTURE_H
