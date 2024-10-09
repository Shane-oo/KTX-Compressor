//
// Created by ShaneMonck on 3/09/2024.
//

#include "KTXTexture.h"

namespace KTXCompressor {

    // #region Private Methods

    void KTXTexture::LoadKtx2File(const string &fileName) {
        ktxTexture2 *ktxTexture;
        auto createKtxTextureResult = ktxTexture_CreateFromNamedFile(fileName.c_str(),
                                                                     KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
                                                                     reinterpret_cast<struct ktxTexture **>(&ktxTexture));
        if (createKtxTextureResult != KTX_SUCCESS) {
            throw runtime_error("Could not Load the Requested .ktx2 Image");
        }

        // Check if the source KTX 2.0 file needs transcoding, 
        // this will transcode the texture data into the GPU native target format
        if (ktxTexture2_NeedsTranscoding(ktxTexture)) {
            ktx_transcode_flags transcodeFlags = 0;
            auto transcodeKtxTextureResult = ktxTexture2_TranscodeBasis(ktxTexture,
                                                                        physicalDevice->GetBestAvailableKTXFormat(),
                                                                        transcodeFlags);
            if (transcodeKtxTextureResult != KTX_SUCCESS) {
                throw runtime_error("Could not Transcode the Input Texture to the Target Format");
            }
        }

        auto width = static_cast<uint32_t>(ktxTexture->baseWidth);
        auto height = static_cast<uint32_t>(ktxTexture->baseHeight);

        // hmm what is this?
        VkFormat format = static_cast<VkFormat>(ktxTexture->vkFormat);

        CreateImage(width, height, ktxTexture->pData);
        sampler = CreateTextureSampler();
    }

    // #endregion

    // #region Constructors

    KTXTexture::KTXTexture(const string &fileName, LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice)
            : Texture(logicalDevice, physicalDevice) {
        this->name = fileName;

        LoadKtx2File(fileName);
    }

    // #endregion

    // #region Destructors

    KTXTexture::~KTXTexture() {
        cout << "Destroying KTX Texture " << name << endl;
        
        vkDestroySampler(logicalDevice->GetVulkanDevice(), sampler, nullptr);

        CleanUpImageView();
    }

    // #endregion

} // KTXCompressor