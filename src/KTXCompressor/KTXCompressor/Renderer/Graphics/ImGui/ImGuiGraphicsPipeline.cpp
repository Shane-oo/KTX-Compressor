//
// Created by ShaneMonck on 4/09/2024.
//

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include "ImGuiGraphicsPipeline.h"
#include "ImGuiRenderPass.h"

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

    RenderPass *ImGuiGraphicsPipeline::CreateRenderPass() {
        return new ImGuiRenderPass(physicalDevice,
                                   logicalDevice,
                                   swapChain->GetImageFormat(),
                                   isFirstToRender,
                                   isLastToRender);
    }

    // #endregion


    // #region Constructors

    ImGuiGraphicsPipeline::ImGuiGraphicsPipeline(
            Window *window,
            Instance *instance,
            PhysicalDevice *physicalDevice,
            LogicalDevice *logicalDevice,
            SwapChain *swapChain,
            bool isFirstToRender,
            bool isLastToRender)
            : GraphicsPipeline(physicalDevice,
                               logicalDevice,
                               swapChain,
                               isFirstToRender,
                               isLastToRender,
                               false) {
        fileSelectorPopUp = new ImGuiFileSelectorPopUp();

        Init();

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
        auto graphicsQueue = logicalDevice->GetGraphicsQueue();
        imGuiImplVulkanInitInfo.QueueFamily = graphicsQueue->GetQueueFamilyIndex();
        imGuiImplVulkanInitInfo.Queue = graphicsQueue->GetVulkanQueue();
        imGuiImplVulkanInitInfo.PipelineCache = VK_NULL_HANDLE; // Optional
        imGuiImplVulkanInitInfo.DescriptorPool = descriptorPool->GetVulkanDescriptorPool();
        imGuiImplVulkanInitInfo.Allocator = nullptr;

        auto imageCount = swapChain->GetImagesCount();
        imGuiImplVulkanInitInfo.MinImageCount = imageCount;
        imGuiImplVulkanInitInfo.ImageCount = imageCount;

        imGuiImplVulkanInitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        imGuiImplVulkanInitInfo.CheckVkResultFn = nullptr;


        bool initSuccessful = ImGui_ImplVulkan_Init(&imGuiImplVulkanInitInfo, renderPass->GetVulkanRenderPass());
        if (!initSuccessful) {
            throw runtime_error("failed to Init Im Gui for Vulkan");
        }

        cout << "Successfully Created ImGui for Vulkan" << endl;
    }

    // #endregion

    // #region Destructors

    ImGuiGraphicsPipeline::~ImGuiGraphicsPipeline() {
        cout << "Destroy ImGui Graphics Pipeline" << endl;

        delete fileSelectorPopUp;

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        delete descriptorPool;
    }


    // #endregion


    // #region Public Methods

    VkCommandBuffer ImGuiGraphicsPipeline::Draw(VkFramebuffer vulkanFrameBuffer, uint32_t currentFrame) {
        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // everything want render must be between ImGui::NewFrame() 
        // and ImGui::Render that happens in the ImGuiRenderPass

        ImGui::Begin(GUI_MAIN_NAME);
        // todo make main window not draggable or moveable...
        
        ImGui::SetWindowSize(ImVec2(250.f, 250.f));
        ImGui::SetWindowPos(ImVec2(0.f, 0.f));
        


        // Create a button to open the file selector
        fileSelectorPopUp->Show();


        ImGui::End(); // ends main window

        return GraphicsPipeline::Draw(vulkanFrameBuffer, currentFrame);
    }


    // #endregion

} // KTXCompressor