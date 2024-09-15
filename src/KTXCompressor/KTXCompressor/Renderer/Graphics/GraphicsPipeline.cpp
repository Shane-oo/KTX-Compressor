//
// Created by shane on 28/07/2024.
//

#include "GraphicsPipeline.h"
#include "../Synchronization/Synchronization.h"

namespace KTXCompressor {

    // #region Constructors

    GraphicsPipeline::GraphicsPipeline(PhysicalDevice *physicalDevice,
                                       LogicalDevice *logicalDevice,
                                       SwapChain *swapChain,
                                       bool isFirstToRender,
                                       bool isLastToRender,
                                       bool usesShader) {
        this->isFirstToRender = isFirstToRender;
        this->isLastToRender = isLastToRender;
        this->usesShader = usesShader;
        this->physicalDevice = physicalDevice;
        this->logicalDevice = logicalDevice;
        this->swapChain = swapChain;
        drawCommand = new DrawCommand(logicalDevice);
    }

    // #endregion

    // #region Destructors 

    GraphicsPipeline::~GraphicsPipeline() {
        cout << "Destroy Graphics Pipeline" << endl;

        delete renderPass;
        if (usesShader) {
            delete shader;
        }

        delete drawCommand;
    }

    // #endregion

    // #region Protected Methods

    void GraphicsPipeline::Init() {
        renderPass = CreateRenderPass();

        if (usesShader) {
            shader = CreateShader();
        }
    }


    // #endregion

    // #region Public Methods

    VkCommandBuffer GraphicsPipeline::Draw(VkFramebuffer vulkanFrameBuffer, uint32_t currentFrame) {
        drawCommand->Begin(currentFrame);

        auto vulkanCommandBuffer = drawCommand->GetVulkanCommandBuffer(currentFrame);

        auto extent = swapChain->GetExtent();
        renderPass->Begin(vulkanCommandBuffer,
                          vulkanFrameBuffer,
                          extent);

        if (usesShader) {
            shader->Bind(vulkanCommandBuffer, currentFrame);
        }

        // Dynamic viewport and scissor
        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(extent.width);
        viewport.height = static_cast<float>(extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(vulkanCommandBuffer, 0, 1, &viewport);

        VkRect2D scissor = {};
        scissor.offset = {0, 0};
        scissor.extent = extent;
        vkCmdSetScissor(vulkanCommandBuffer, 0, 1, &scissor);

        if (usesShader) {
            shader->Render(vulkanCommandBuffer, currentFrame, extent);
        }

        renderPass->Render(vulkanCommandBuffer);

        renderPass->End(vulkanCommandBuffer);

        drawCommand->End(currentFrame);

        return drawCommand->GetVulkanCommandBuffer(currentFrame);
    }

    // #endregion

} // KTXCompressor