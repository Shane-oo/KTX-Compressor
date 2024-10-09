//
// Created by ShaneMonck on 27/08/2024.
//

#ifndef KTX_COMPRESSOR_COMBINEDIMAGESAMPLERDESCRIPTORSET_H
#define KTX_COMPRESSOR_COMBINEDIMAGESAMPLERDESCRIPTORSET_H

#include "DescriptorSet.h"
#include "../../Textures/Texture.h"

namespace KTXCompressor {

    class CombinedImageSamplerDescriptorSet : public DescriptorSet {

    public:
        CombinedImageSamplerDescriptorSet(LogicalDevice *logicalDevice, Texture *texture);
        
        ~CombinedImageSamplerDescriptorSet();

    protected:
        VkDescriptorSetLayoutBinding GetDescriptorSetLayoutBinding() override;

        DescriptorPoolSizeModel GetDescriptorPoolSize() override;

        void SetWriteDescriptorSet(VkWriteDescriptorSet &writeDescriptorSet,
                                   size_t i) override;

        int GetBinding() override {
            return binding;
        }

    private:
        int binding = 1;

        Texture *texture;
        vector<VkDescriptorImageInfo> descriptorImageInfos;

    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_COMBINEDIMAGESAMPLERDESCRIPTORSET_H
