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
        ModelViewProjectionDescriptorSet(LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice);

        ~ModelViewProjectionDescriptorSet();

        void Update(uint32_t currentFrame, VkExtent2D extent);

    protected:
        VkDescriptorSetLayout CreateDescriptorSetLayout() override;
        
        VkDescriptorPool CreateDescriptorPool() override;

        vector<VkDescriptorSet> CreateDescriptorSets() override;

    private:
        struct ModelViewProjectionUbo {
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 proj;
        };

        BufferUtil *bufferUtil;
        vector<VkBuffer> vulkanUniformBuffers;
        vector<VkDeviceMemory> vulkanUniformBuffersMemory;
        vector<void *> vulkanUniformBuffersMapped;

        void CreateModelViewProjectionUniformBuffers();
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_MODELVIEWPROJECTIONDESCRIPTORSET_H
