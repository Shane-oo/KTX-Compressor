//
// Created by shane on 29/06/2024.
//

#include <filesystem>
#include "Texture.h"


namespace KTXCompressor {
    // #region Private Methods

    void Texture::OpenImage(const string &fileName,
                            unique_ptr<ImageInput> &imageInput,
                            ImageSpec &resultantImageSpec) {
        imageInput = ImageInput::open(fileName);
        if (!imageInput) {
            throw runtime_error("Could not open file with name" + fileName);
        }
        // Loading multiple sub image from the ame input is not supported // so always go to first sub image and first mipLevel
        imageInput->seek_subimage(0, 0);


        const ImageSpec &imageSpec = imageInput->spec();

        cout << "image width: " << imageSpec.width << " image height: " << imageSpec.height << endl;
        cout << "image channels: " << imageSpec.nchannels << " image bytes: "
             << imageSpec.width * imageSpec.height * imageSpec.nchannels << endl;
        cout << "image depth: " << imageSpec.depth << " image formatDesc: " << imageSpec.format << endl;

        resultantImageSpec = ImageSpec{
                imageSpec.width,
                imageSpec.height,
                imageSpec.nchannels,
                imageSpec.format // not sure about this, comes from options in ktx command create
        };
    }


    std::unique_ptr<unsigned char[]> Texture::LoadInputImage(ImageInput &inputImage, ImageSpec imageSpec) {
        auto pixels = std::unique_ptr<unsigned char[]>(
                new unsigned char[imageSpec.width * imageSpec.height * imageSpec.nchannels]
        );
        int nChannels = imageSpec.nchannels;  // number of image channels, e.g., 4 for RGBA

        inputImage.read_image(0, 0, 0, nChannels, TypeDesc::UINT8,
                              pixels.get());  // Use pixels.get() to access the raw pointer

        return pixels;
    }

    void Texture::CreateKtxTexture(ImageInput &imageInput, const ImageSpec &imageSpec) {
        ktxTextureCreateInfo createInfo;
        // set all createInfo's bytes to 0, prevents uninitialized memory usage
        memset(&createInfo, 0, sizeof(createInfo));

        // magic number 29 seems to be the one for R8G8B8_SRGB, which is diffuse im pretty sure
        createInfo.vkFormat = VK_FORMAT_R8G8B8_SRGB;
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
                                                                     &myKtxTexture);
        if (ktxTextureCreateResult != KTX_SUCCESS) {
            auto error = ktxErrorString(ktxTextureCreateResult);
            throw std::runtime_error("Could not create Ktx Texture: " + string(error));
        }
        cout << "Successfully created Ktx Texture2" << endl;

        auto pixels = LoadInputImage(imageInput, imageSpec);

        const auto setImageFromMemoryResult = ktxTexture_SetImageFromMemory(ktxTexture(myKtxTexture),
                                                                            0,
                                                                            0,
                                                                            0,
                                                                            pixels.get(),
                                                                            imageSpec.width * imageSpec.height *
                                                                            imageSpec.nchannels // 4096x4096x(RGB=>3)
        );

        if (setImageFromMemoryResult != KTX_SUCCESS) {
            auto error = ktxErrorString(setImageFromMemoryResult);
            throw std::runtime_error("Could not set image from memory: " + string(error));
        }

        cout << "Successfully Set Image From Memory" << endl;
    }


    // #endregion

    // #region Constructors 

    Texture::Texture(const string &fileName) {

        unique_ptr<ImageInput> imageInput;
        ImageSpec imageSpec;
        OpenImage(fileName, imageInput, imageSpec);

        CreateKtxTexture(*imageInput, imageSpec);

        // UASTC Compression 
        // note there exists an extended params version
        int astcQuality = 0;
        const auto compressAstcResult = ktxTexture2_CompressAstc(myKtxTexture, astcQuality);
        if (compressAstcResult != KTX_SUCCESS) {
            auto error = ktxErrorString(compressAstcResult);
            throw std::runtime_error("ktxTexture2_CompressAstcEx: " + string(error));
        }

        cout << "Successfully Compressed using Astc" << endl;

        // You are strongly encouraged to also specify --zstd to losslessly compress the UASTC data
        // Supercompress the data with Zstandard. Cannot be used with ETC1S / BasisLZ format. Level range is [1,22]. 
        // Lower levels give faster but worse compression. Values above 20 should be used with caution as they require more memory. 
        int deflateZstdLevel = 5;
        const auto deflateZstdResult = ktxTexture2_DeflateZstd(myKtxTexture, deflateZstdLevel);
        if (deflateZstdResult != KTX_SUCCESS) {
            auto error = ktxErrorString(deflateZstdResult);
            throw std::runtime_error("ktxTexture2_DeflateZstd: " + string(error));
        }

        cout << "Successfully Deflated Astc Ktx Texture" << endl;

        // have to do this?
        // Add KTXwriterScParams metadata if ASTC encoding, BasisU encoding, or other supercompression was used
        const auto writerScParams = fmt::format("{}{}", astcQuality, deflateZstdLevel);
        if (!writerScParams.empty()) {
            const auto addKvPairResult = ktxHashList_AddKVPair(&myKtxTexture->kvDataHead,
                                                               KTX_WRITER_SCPARAMS_KEY,
                                                               static_cast<uint32_t>(writerScParams.size()),
                                                               writerScParams.c_str() +
                                                               1); // +1 to exclude leading space
            if (addKvPairResult != KTX_SUCCESS) {
                auto error = ktxErrorString(addKvPairResult);
                throw std::runtime_error("ktxHashList_AddKVPair: " + string(error));
            }
            cout << "Successfully wrote to Ktx Texture DataHead the metadata used" << endl;
        }

        cout << myKtxTexture->isCompressed << endl;

        // Download

        auto newPixels = std::unique_ptr<unsigned char[]>(new unsigned char[myKtxTexture->dataSize]);
        unsigned char *rawPixels = newPixels.get();

        ktx_size_t writtenSize;
        const auto writeToMemoryResult = ktxTexture_WriteToMemory(ktxTexture(myKtxTexture),
                                                                  &rawPixels,
                                                                  &writtenSize);
        if (writeToMemoryResult != KTX_SUCCESS) {
            auto error = ktxErrorString(writeToMemoryResult);
            throw std::runtime_error("ktxTexture_WriteToMemory: " + string(error));
        }


        // Open file for writing
        std::ofstream outputFile("my_new_texture.ktx2", std::ios::binary);
        if (!outputFile.is_open()) {
            std::cerr << "Failed to open file for writing: " << "my_new_texture.ktx2" << std::endl;
            return; // Error
        }
        // Write pixel data
        outputFile.write(reinterpret_cast<const char *>(newPixels.get()), writtenSize);

        // TODO: Does a Ktx viewer not exist? -> Make own, easy do in three js. This can be the base site for Ktx-Compressor
        // am writing to my_new_texture.ktx2 and it has a bytes size of 5mb, so promising, however dont know if its
        // actually working or not

        outputFile.close();

        imageInput->close();
    }

    Texture::~Texture() {
        if (myKtxTexture) {
            ktxTexture_Destroy(ktxTexture(myKtxTexture));
        }

    }

    // #endregion


} // KTXCompressor