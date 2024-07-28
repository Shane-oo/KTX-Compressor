//
// Created by shane on 28/07/2024.
//

#ifndef KTXCOMPRESSOR_GRAPHICSPIPELINE_H
#define KTXCOMPRESSOR_GRAPHICSPIPELINE_H

#include "../../Common.h"
#include "Shader.h"
#include "../Presentation/SwapChain.h"
#include "RenderPass.h"

namespace KTXCompressor {

    class GraphicsPipeline {
    public:
        GraphicsPipeline(VkDevice device, SwapChain *swapChain);

        ~GraphicsPipeline();

    private:
        VkPipeline CreateVulkanGraphicsPipeline();

        Shader *shader;
    protected:
        VkDevice vulkanDevice;
        SwapChain *swapChain;
  
        RenderPass *renderPass;
        VkPipeline vulkanGraphicsPipeline;

    protected:
        virtual Shader *CreateShader() = 0;

        void Init();

        virtual void
        SetRasterizationStateCreateInfo(VkPipelineRasterizationStateCreateInfo &rasterizationStateCreateInfo) = 0;
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_GRAPHICSPIPELINE_H
