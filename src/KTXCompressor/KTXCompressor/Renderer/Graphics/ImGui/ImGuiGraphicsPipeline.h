//
// Created by ShaneMonck on 4/09/2024.
//

#ifndef KTX_COMPRESSOR_IMGUIGRAPHICSPIPELINE_H
#define KTX_COMPRESSOR_IMGUIGRAPHICSPIPELINE_H

#include "../GraphicsPipeline.h"
#include "PopUps/ImGuiFileSelectorPopUp.h"

namespace KTXCompressor {

    class ImGuiGraphicsPipeline : public GraphicsPipeline {
    public:
        ImGuiGraphicsPipeline(Window *window,
                              Instance *instance,
                              PhysicalDevice *physicalDevice,
                              LogicalDevice *logicalDevice,
                              SwapChain *swapChain,
                              bool isFirstToRender,
                              bool isLastToRender);

        ~ImGuiGraphicsPipeline() override;

        VkCommandBuffer Draw(VkFramebuffer vulkanFrameBuffer, uint32_t currentFrame) override;

    protected:
        Shader *CreateShader() override;

        RenderPass *CreateRenderPass() override;

    private:
        DescriptorPool *descriptorPool;
        ImGuiFileSelectorPopUp *fileSelectorPopUp;


        DescriptorPool *CreateImGuiDescriptorPool();
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_IMGUIGRAPHICSPIPELINE_H
