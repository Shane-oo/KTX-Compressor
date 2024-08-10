//
// Created by shane on 28/07/2024.
//

#ifndef KTXCOMPRESSOR_SIMPLETRIANGLESHADER_H
#define KTXCOMPRESSOR_SIMPLETRIANGLESHADER_H

#include "../Shader.h"

namespace KTXCompressor {

    class SimpleTriangleShader : public Shader {

    public:
        SimpleTriangleShader(PhysicalDevice *physicalDevice, LogicalDevice *logicalDevice);

        void Render(VkCommandBuffer vulkanCommandBuffer) override;

    private:
        const vector<Vertex> vertices = {
                {{-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
                {{0.5f,  -0.5f}, {1.0f, 1.0f, 0.0f}},
                {{0.5f,  0.5f},  {0.0f, 0.0f, 1.0f}},
                {{-0.5f, 0.5f},  {0.0f, 1.0f, 1.0f}}
        };

        const vector<uint16_t> indices = {
                0, 1, 2, 2, 3, 0
        };

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
