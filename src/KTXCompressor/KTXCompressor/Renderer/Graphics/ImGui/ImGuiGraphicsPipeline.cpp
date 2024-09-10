//
// Created by ShaneMonck on 4/09/2024.
//

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include "ImGuiGraphicsPipeline.h"

namespace KTXCompressor {
    // #region Protected Methods

    Shader *ImGuiGraphicsPipeline::CreateShader() {
        return nullptr;
    }

    void ImGuiGraphicsPipeline::SetRasterizationStateCreateInfo(
            VkPipelineRasterizationStateCreateInfo &rasterizationStateCreateInfo) {

    }

    // #endregion


    // #region Constructors

    ImGuiGraphicsPipeline::ImGuiGraphicsPipeline(
            Window *window,
            Instance *instance,
            PhysicalDevice *physicalDevice,
            LogicalDevice *logicalDevice,
            SwapChain *swapChain,
            uint32_t graphicsFamilyIndex)
            : GraphicsPipeline(physicalDevice,
                               logicalDevice,
                               swapChain,
                               graphicsFamilyIndex,
                               false,
                               true // imgui should always be last...
    ) {
        // Setup Dear ImGui Context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        // Setup Dear ImGui Style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer Bindings
        ImGui_ImplGlfw_InitForVulkan(window->GetGlfwWindow(), true);
        ImGui_ImplVulkan_InitInfo imGuiImplVulkanInitInfo = {};
        imGuiImplVulkanInitInfo.Instance = instance->GetVulkanInstance();
        imGuiImplVulkanInitInfo.PhysicalDevice = physicalDevice->GetVulkanPhysicalDevice();
        imGuiImplVulkanInitInfo.Device = logicalDevice->GetVulkanDevice();
        imGuiImplVulkanInitInfo.QueueFamily = graphicsFamilyIndex;
        imGuiImplVulkanInitInfo.PipelineCache = VK_NULL_HANDLE; // Optional
        imGuiImplVulkanInitInfo.DescriptorPool = todo;
        imGuiImplVulkanInitInfo.Allocator = nullptr;

        auto imageCount = swapChain->GetImagesCount();
        imGuiImplVulkanInitInfo.MinImageCount = imageCount;
        imGuiImplVulkanInitInfo.ImageCount = imageCount;

        imGuiImplVulkanInitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        imGuiImplVulkanInitInfo.CheckVkResultFn = nullptr;

        bool initSuccessful = ImGui_ImplVulkan_Init(&imGuiImplVulkanInitInfo, todoRenderPass);
        if (!initSuccessful) {
            throw runtime_error("failed to Init Im Gui for Vulkan");
        }
    }


    // #endregion
} // KTXCompressor