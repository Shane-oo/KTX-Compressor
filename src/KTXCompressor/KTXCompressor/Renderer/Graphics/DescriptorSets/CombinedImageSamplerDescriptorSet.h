//
// Created by ShaneMonck on 27/08/2024.
//

#ifndef KTX_COMPRESSOR_COMBINEDIMAGESAMPLERDESCRIPTORSET_H
#define KTX_COMPRESSOR_COMBINEDIMAGESAMPLERDESCRIPTORSET_H

#include "DescriptorSet.h"
#include "../Textures/Texture.h"

namespace KTXCompressor {

    class CombinedImageSamplerDescriptorSet : public DescriptorSet {

    public:
        CombinedImageSamplerDescriptorSet(LogicalDevice *logicalDevice, Texture *texture);
        
        void BindToCommandBuffer(VkCommandBuffer vulkanCommandBuffer,
                                      VkPipelineLayout vulkanPipelineLayout,
                                      uint32_t currentFrame) override;

    protected:
        VkDescriptorSetLayoutBinding GetDescriptorSetLayoutBinding() override;

        VkDescriptorPoolSize GetDescriptorPoolSize() override;

        void SetWriteDescriptorSet(VkWriteDescriptorSet &writeDescriptorSet,
                                   size_t i) override;

    private:
        Texture *texture;
        vector<VkDescriptorImageInfo> descriptorImageInfos;

    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_COMBINEDIMAGESAMPLERDESCRIPTORSET_H
