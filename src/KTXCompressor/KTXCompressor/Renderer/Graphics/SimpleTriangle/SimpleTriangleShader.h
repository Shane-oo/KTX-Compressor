//
// Created by shane on 28/07/2024.
//

#ifndef KTXCOMPRESSOR_SIMPLETRIANGLESHADER_H
#define KTXCOMPRESSOR_SIMPLETRIANGLESHADER_H

#include "../Shader.h"

namespace KTXCompressor {

    class SimpleTriangleShader : public Shader {

    public:
        SimpleTriangleShader(PhysicalDevice *physicalDevice, VkDevice device);

        void Render(VkCommandBuffer vulkanCommandBuffer) override;

    private:
        const vector<Vertex> vertices = {
                {{0.0f,  -0.5f}, {1.0f, 1.0f, 1.0f}},
                {{0.5f,  0.5f},  {0.0f, 1.0f, 0.0f}},
                {{-0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}}
        };

    public:
        const char *GetVertexEntryPointName() override;

        const char *GetFragmentEntryPointName() override;

    protected:
        VkPipelineLayout CreatePipelineLayout() override;

        VkBuffer CreateVertexBuffer() override;

        void FillVertexBuffer() override;
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_SIMPLETRIANGLESHADER_H
