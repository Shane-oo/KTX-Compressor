//
// Created by ShaneMonck on 27/08/2024.
//

#include "CombinedImageSamplerDescriptorSet.h"
#include "../../RendererConstants.h"

namespace KTXCompressor {

    // #region Constructors

    CombinedImageSamplerDescriptorSet::CombinedImageSamplerDescriptorSet(LogicalDevice *logicalDevice, Texture *texture)
            :
            DescriptorSet(logicalDevice) {
        this->texture = texture;
        Init();
    }

    // #endregion

    // #region Protected Methods


    VkDescriptorSetLayoutBinding CombinedImageSamplerDescriptorSet::GetDescriptorSetLayoutBinding() {
        VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.descriptorCount = 1; // not an array
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional

        return samplerLayoutBinding;
    }

    VkDescriptorPoolSize CombinedImageSamplerDescriptorSet::GetDescriptorPoolSize() {
        VkDescriptorPoolSize descriptorPoolSize = {};
        descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorPoolSize.descriptorCount = static_cast<uint32_t>(RendererConstants::MAX_FRAMES_IN_FLIGHT);

        return descriptorPoolSize;
    }

    void CombinedImageSamplerDescriptorSet::SetWriteDescriptorSet(VkWriteDescriptorSet &writeDescriptorSet, size_t i) {
        if (descriptorImageInfos.size() <= i) {
            descriptorImageInfos.resize(i + 1);
        }
        descriptorImageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        descriptorImageInfos[i].imageView = texture->GetImageView()->GetVulkanImageView();
        descriptorImageInfos[i].sampler = texture->GetSampler();

        writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.dstBinding = 1;
        writeDescriptorSet.dstArrayElement = 0;
        writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeDescriptorSet.descriptorCount = 1;
        writeDescriptorSet.pImageInfo = &descriptorImageInfos[i];
    }

    // #endregion

    // #region Public Methods

    void CombinedImageSamplerDescriptorSet::BindToCommandBuffer(VkCommandBuffer vulkanCommandBuffer,
                                                                VkPipelineLayout vulkanPipelineLayout,
                                                                uint32_t currentFrame) {
        vkCmdBindDescriptorSets(vulkanCommandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                vulkanPipelineLayout,
                                1,
                                1, // CombinedImageSampler is the second one...?
                                &vulkanDescriptorSets[currentFrame], 0, nullptr);
    }


    // #endregion

} // KTXCompressor