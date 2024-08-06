//
// Created by shane on 28/07/2024.
//

#ifndef KTXCOMPRESSOR_GRAPHICSPIPELINE_H
#define KTXCOMPRESSOR_GRAPHICSPIPELINE_H

#include "../../Common.h"
#include "Shader.h"
#include "../Presentation/SwapChain.h"
#include "RenderPass.h"
#include "../Commands/DrawCommand.h"

namespace KTXCompressor {

    class GraphicsPipeline {
    public:
        GraphicsPipeline(VkDevice device, SwapChain *swapChain, uint32_t graphicsFamilyIndex);

        ~GraphicsPipeline();

        void Draw(VkFramebuffer vulkanFrameBuffer);

    private:
        VkPipeline CreateVulkanGraphicsPipeline();

        Shader *shader;
    protected:
        VkDevice vulkanDevice;
        SwapChain *swapChain;
        RenderPass *renderPass;
        DrawCommand *drawCommand;
        VkPipeline vulkanGraphicsPipeline;


        virtual Shader *CreateShader() = 0;

        void Init();

        virtual void
        SetRasterizationStateCreateInfo(VkPipelineRasterizationStateCreateInfo &rasterizationStateCreateInfo) = 0;

        virtual void Render() = 0;

    public:
        RenderPass *GetRenderPass() {
            return renderPass;
        }

        VkPipeline GetVulkanGraphicsPipeline() {
            return vulkanGraphicsPipeline;
        }
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_GRAPHICSPIPELINE_H
