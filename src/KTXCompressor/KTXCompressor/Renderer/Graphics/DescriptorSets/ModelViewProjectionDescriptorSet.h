//
// Created by ShaneMonck on 13/08/2024.
//

#ifndef KTX_COMPRESSOR_MODELVIEWPROJECTIONDESCRIPTORSET_H
#define KTX_COMPRESSOR_MODELVIEWPROJECTIONDESCRIPTORSET_H


#define GLM_FORCE_RADIANS

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

#include "DescriptorSet.h"
#include "../../Utils/BufferUtil.h"

namespace KTXCompressor {

    class ModelViewProjectionDescriptorSet : public DescriptorSet {
    public:
        ModelViewProjectionDescriptorSet(LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice, uint32_t speed);

        ~ModelViewProjectionDescriptorSet();

        void Update(uint32_t currentFrame, VkExtent2D extent);

    protected:
        VkDescriptorSetLayoutBinding GetDescriptorSetLayoutBinding() override;

        VkDescriptorPoolSize GetDescriptorPoolSize() override;

        void SetWriteDescriptorSet(VkWriteDescriptorSet &writeDescriptorSet,
                                   size_t i) override;


        int GetBinding() override {
            return binding;
        }

    private:
        int binding = 0;

        uint32_t speed;
        struct ModelViewProjectionUbo {
            alignas(16)  glm::mat4 model;
            alignas(16) glm::mat4 view;
            alignas(16) glm::mat4 proj;
        };

        BufferUtil *bufferUtil;
        vector<VkBuffer> vulkanUniformBuffers;
        vector<VkDeviceMemory> vulkanUniformBuffersMemory;
        vector<void *> vulkanUniformBuffersMapped;
        vector<VkDescriptorBufferInfo> descriptorBufferInfos;

        void CreateModelViewProjectionUniformBuffers();
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_MODELVIEWPROJECTIONDESCRIPTORSET_H
