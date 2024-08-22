//
// Created by shane on 28/07/2024.
//

#ifndef KTXCOMPRESSOR_SIMPLETRIANGLESHADER_H
#define KTXCOMPRESSOR_SIMPLETRIANGLESHADER_H

#include "../Shader.h"
#include "../DescriptorSets/ModelViewProjectionDescriptorSet.h"
#include "../Textures/Texture.h"

namespace KTXCompressor {

    class SimpleTriangleShader : public Shader {

    public:
        SimpleTriangleShader(PhysicalDevice *physicalDevice, LogicalDevice *logicalDevice);
        
        ~SimpleTriangleShader();

        void Render(VkCommandBuffer vulkanCommandBuffer, uint32_t currentFrame, VkExtent2D extent) override;

    private:
        const vector<Vertex> vertices = {
                {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                {{0.5f,  -0.5f}, {0.0f, 1.0f, 0.0f}},
                {{0.5f,  0.5f},  {0.0f, 0.0f, 1.0f}},
                {{-0.5f, 0.5f},  {1.0f, 1.0f, 1.0f}}
        };

        const vector<uint16_t> indices = {
                0, 1, 2, 2, 3, 0
        };

        ModelViewProjectionDescriptorSet *modelViewProjectionDescriptorSet;
        Texture* woodTexture;

    public:
        const char *GetVertexEntryPointName() override;

        const char *GetFragmentEntryPointName() override;

    protected:
        VkPipelineLayout CreatePipelineLayout() override;

        void CreateVertexBuffer() override;

        void CreateIndexBuffer() override;

        void BindDescriptorSet(VkCommandBuffer vulkanCommandBuffer, uint32_t currentFrame) override;
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_SIMPLETRIANGLESHADER_H
