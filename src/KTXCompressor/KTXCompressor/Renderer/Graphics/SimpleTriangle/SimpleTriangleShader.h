//
// Created by shane on 28/07/2024.
//

#ifndef KTXCOMPRESSOR_SIMPLETRIANGLESHADER_H
#define KTXCOMPRESSOR_SIMPLETRIANGLESHADER_H

#include "../Shader.h"
#include "../DescriptorSets/ModelViewProjectionDescriptorSet.h"
#include "../../Textures/Texture.h"
#include "../DescriptorSets/CombinedImageSamplerDescriptorSet.h"
#include "../../Textures/KTXTexture.h"
#include "../../Textures/ImageTexture.h"

namespace KTXCompressor {

    class SimpleTriangleShader : public Shader {

    public:
        SimpleTriangleShader(PhysicalDevice *physicalDevice, LogicalDevice *logicalDevice, uint32_t speed);

        ~SimpleTriangleShader();

        void Render(VkCommandBuffer vulkanCommandBuffer, uint32_t currentFrame, VkExtent2D extent) override;

    private:
        uint32_t speed;
        vector<Vertex> vertices = {
                {{-0.5f, -0.5f, 0.0f},  {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                {{0.5f,  -0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
                {{0.5f,  0.5f,  0.0f},  {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
                {{-0.5f, 0.5f,  0.0f},  {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

                {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                {{0.5f,  -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
                {{0.5f,  0.5f,  -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
                {{-0.5f, 0.5f,  -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
        };

        const vector<uint16_t> indices = {
                0, 1, 2, 2, 3, 0,
                4, 5, 6, 6, 7, 4
        };

        ModelViewProjectionDescriptorSet *modelViewProjectionDescriptorSet;
        CombinedImageSamplerDescriptorSet *combinedImageSamplerDescriptorSet;
        KTXTexture *ktxTexture;
        ImageTexture *imageTexture;

    public:
        const char *GetVertexEntryPointName() override;

        const char *GetFragmentEntryPointName() override;

    protected:
        VkPipelineLayout CreatePipelineLayout() override;

        void CreateVertexBuffer() override;

        void CreateIndexBuffer() override;
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_SIMPLETRIANGLESHADER_H
