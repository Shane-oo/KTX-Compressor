//
// Created by shane on 28/07/2024.
//

#ifndef KTXCOMPRESSOR_GRAPHICSPIPELINE_H
#define KTXCOMPRESSOR_GRAPHICSPIPELINE_H

#include "../../Common.h"
#include "Shader.h"
#include "../Presentation/SwapChain.h"
#include "RenderPasses/RenderPass.h"
#include "../Commands/DrawCommand.h"
#include "../Synchronization/Synchronization.h"

namespace KTXCompressor {

    class GraphicsPipeline {
    public:
        GraphicsPipeline(PhysicalDevice *physicalDevice,
                         LogicalDevice *logicalDevice,
                         SwapChain *swapChain,
                         uint32_t graphicsFamilyIndex,
                         bool isFirstToRender,
                         bool isLastToRender);

        ~GraphicsPipeline();

        VkCommandBuffer Draw(VkFramebuffer vulkanFrameBuffer, uint32_t currentFrame);

    private:
        VkPipeline CreateVulkanGraphicsPipeline();

        Shader *shader;
    protected:
        PhysicalDevice *physicalDevice;
        LogicalDevice *logicalDevice;
        SwapChain *swapChain;
        RenderPass *renderPass;
        DrawCommand *drawCommand;
        VkPipeline vulkanGraphicsPipeline;
        bool isFirstToRender;
        bool isLastToRender;


        virtual Shader *CreateShader() = 0;

        virtual RenderPass *CreateRenderPass() = 0;

        void Init();

        virtual void
        SetRasterizationStateCreateInfo(VkPipelineRasterizationStateCreateInfo &rasterizationStateCreateInfo) = 0;

    public:
        RenderPass *GetRenderPass() {
            return renderPass;
        }

        Shader *GetShader() {
            return shader;
        }

        VkPipeline GetVulkanGraphicsPipeline() {
            return vulkanGraphicsPipeline;
        }
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_GRAPHICSPIPELINE_H
