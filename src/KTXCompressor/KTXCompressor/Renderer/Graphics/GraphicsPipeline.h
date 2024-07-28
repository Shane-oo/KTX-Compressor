//
// Created by shane on 28/07/2024.
//

#ifndef KTXCOMPRESSOR_GRAPHICSPIPELINE_H
#define KTXCOMPRESSOR_GRAPHICSPIPELINE_H

#include "../../Common.h"
#include "Shader.h"

namespace KTXCompressor {

    class GraphicsPipeline {
    public:
        GraphicsPipeline(VkDevice device);
        
        ~GraphicsPipeline();

    protected:
        Shader *shader;
        VkDevice vulkanDevice;

    protected:
        virtual Shader *CreateShader() = 0;

        void Init();
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_GRAPHICSPIPELINE_H
