//
// Created by shane on 28/07/2024.
//

#ifndef KTXCOMPRESSOR_SIMPLETRIANGLESHADER_H
#define KTXCOMPRESSOR_SIMPLETRIANGLESHADER_H

#include "../../Shader.h"

namespace KTXCompressor {

    class SimpleTriangleShader : public Shader {
    public:
        SimpleTriangleShader(VkDevice device);

    public:
        const char *GetVertexEntryPointName() override;

        const char *GetFragmentEntryPointName() override;

        VkPipelineLayout CreatePipelineLayout() override;
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_SIMPLETRIANGLESHADER_H
