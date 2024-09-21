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
                         bool isFirstToRender,
                         bool isLastToRender,
                         bool usesShader);

        virtual ~GraphicsPipeline();

        virtual VkCommandBuffer Draw(VkFramebuffer vulkanFrameBuffer, uint32_t currentFrame);

    private:

        Shader *shader;
    protected:
        PhysicalDevice *physicalDevice;
        LogicalDevice *logicalDevice;
        SwapChain *swapChain;
        RenderPass *renderPass;
        DrawCommand *drawCommand;
        bool isFirstToRender;
        bool isLastToRender;
        bool usesShader;


        virtual Shader *CreateShader() = 0;

        virtual RenderPass *CreateRenderPass() = 0;

        void Init();


    public:
        RenderPass *GetRenderPass() {
            return renderPass;
        }

        Shader *GetShader() {
            return shader;
        }

        bool GetIsFirstToRender() {
            return isFirstToRender;
        }

        bool GetIsLastToRender() {
            return isLastToRender;
        }
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_GRAPHICSPIPELINE_H
