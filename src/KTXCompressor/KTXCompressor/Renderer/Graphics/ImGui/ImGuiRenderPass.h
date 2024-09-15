//
// Created by ShaneMonck on 15/09/2024.
//

#ifndef KTX_COMPRESSOR_IMGUIRENDERPASS_H
#define KTX_COMPRESSOR_IMGUIRENDERPASS_H

#include <imgui.h>
#include "../RenderPasses/RenderPass.h"
#include "../../../Common.h"

namespace KTXCompressor {

    class ImGuiRenderPass : public RenderPass {
    public:
        ImGuiRenderPass(PhysicalDevice *physicalDevice,
                        LogicalDevice *logicalDevice,
                        VkFormat swapChainImageFormat,
                        bool isFirstRenderPass,
                        bool isLastRenderPass);

        bool NeedsDepthAttachment() override {
            return false;
        }

        void Render(VkCommandBuffer vulkanCommandBuffer) override;

    protected:
        VkRenderPass CreateVulkanRenderPass() override;

        void SetClearValues(VkRenderPassBeginInfo &renderPassBeginInfo) override;


    private:
        VkClearValue vulkanClearValue;
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_IMGUIRENDERPASS_H
