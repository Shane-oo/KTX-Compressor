//
// Created by ShaneMonck on 4/09/2024.
//

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include "ImGuiGraphicsPipeline.h"

namespace KTXCompressor {
    // #region Private Methods

    DescriptorPool *ImGuiGraphicsPipeline::CreateImGuiDescriptorPool() {
        vector<VkDescriptorPoolSize> descriptorPoolSizes = {
                {VK_DESCRIPTOR_TYPE_SAMPLER,                1000},
                {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000}};

        auto descriptorPoolSizeModel = DescriptorPoolSizeModel(descriptorPoolSizes,
                                                               VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
                                                               1);

        return new DescriptorPool(logicalDevice, descriptorPoolSizeModel);
    }

    // #endregion

    // #region Protected Methods

    Shader *ImGuiGraphicsPipeline::CreateShader() {
        return nullptr;
    }

    void ImGuiGraphicsPipeline::SetRasterizationStateCreateInfo(
            VkPipelineRasterizationStateCreateInfo &rasterizationStateCreateInfo) {

    }

    RenderPass *ImGuiGraphicsPipeline::CreateRenderPass() {
        return nullptr;
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

        descriptorPool = CreateImGuiDescriptorPool();




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
        imGuiImplVulkanInitInfo.DescriptorPool = descriptorPool->GetVulkanDescriptorPool();
        imGuiImplVulkanInitInfo.Allocator = nullptr;

        auto imageCount = swapChain->GetImagesCount();
        imGuiImplVulkanInitInfo.MinImageCount = imageCount;
        imGuiImplVulkanInitInfo.ImageCount = imageCount;

        imGuiImplVulkanInitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        imGuiImplVulkanInitInfo.CheckVkResultFn = nullptr;

       /* bool initSuccessful = ImGui_ImplVulkan_Init(&imGuiImplVulkanInitInfo, todoRenderPass);
        if (!initSuccessful) {
            throw runtime_error("failed to Init Im Gui for Vulkan");
        }*/
    }



    // #endregion
} // KTXCompressor