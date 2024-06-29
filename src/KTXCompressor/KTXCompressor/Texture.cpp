//
// Created by shane on 29/06/2024.
//

#include "Texture.h"


namespace KTXCompressor {
    // #region Private Methods

    void Texture::CreateKtxTexture(const ImageSpec &imageSpec) {
        ktxTextureCreateInfo createInfo;
        // set all createInfo's bytes to 0, prevents uninitialized memory usage
        memset(&createInfo, 0, sizeof(createInfo));

        // magic number 29 seems to be the one for R8G8B8_SRGB, which is diffuse im pretty sure
        createInfo.vkFormat = VK_FORMAT_R8G8B8A8_SRGB;
        createInfo.numFaces = 1; // 6 for cube maps, 1 otherwise
        createInfo.numLayers = 1;// number of array layers , will throw if 0
        createInfo.isArray = false; // not an array
        createInfo.baseWidth = imageSpec.width;
        createInfo.baseHeight = imageSpec.height;
        createInfo.baseDepth = imageSpec.depth; // is this 1?
        createInfo.numDimensions = 2; // just x and y i think
        createInfo.generateMipmaps = false; // for now no mip maps
        createInfo.numLevels = 1; // number of mip levels, should be 1 if generateMipmaps is true, but will throw if 0??

        ktx_error_code_e ktxTextureCreateResult = ktxTexture2_Create(&createInfo,
                                                                     KTX_TEXTURE_CREATE_ALLOC_STORAGE,
                                                                     &ktxTexture);
        if (ktxTextureCreateResult != KTX_SUCCESS) {

            auto error = ktxErrorString(ktxTextureCreateResult);
            cout << "ERROR:" << error << endl;
            return;
        }


    }

    // #endregion

    // #region Constructors 

    Texture::Texture(const string &fileName) {
        auto woodDiffuseImageInput = ImageInput::open(fileName);
        if (!woodDiffuseImageInput) {
            throw std::runtime_error("Could not open file with name" + fileName);
        }

        // Loading multiple sub image from the ame input is not supported
        // so always go to first sub image and first mipLevel
        woodDiffuseImageInput->seek_subimage(0, 0);


        const ImageSpec &woodImageSpec = woodDiffuseImageInput->spec();

        std::cout << "image width: " << woodImageSpec.width << "image height: " << woodImageSpec.height << std::endl;
        std::cout << "image depth: " << woodImageSpec.depth << "image formatDesc: " << woodImageSpec.format
                  << std::endl;


        ImageSpec woodImageSpecModified = ImageSpec{
                woodImageSpec.width,
                woodImageSpec.height,
                woodImageSpec.depth, // note: should default to 1 if null
                woodImageSpec.format // not sure about this, comes from options in ktx command create
        };

        CreateKtxTexture(woodImageSpecModified);

        woodDiffuseImageInput->close();
    }

    // #endregion


} // KTXCompressor