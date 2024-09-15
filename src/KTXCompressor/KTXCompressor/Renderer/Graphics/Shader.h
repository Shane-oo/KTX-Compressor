//
// Created by shane on 28/07/2024.
//

#ifndef KTXCOMPRESSOR_SHADER_H
#define KTXCOMPRESSOR_SHADER_H

#include <vulkan/vulkan_core.h>
#include <glm/glm.hpp>
#include "../../Common.h"
#include "../Instance/Devices/PhysicalDevice.h"
#include "../Instance/Devices/LogicalDevice.h"
#include "../Utils/BufferUtil.h"
#include "DescriptorSets/DescriptorSet.h"
#include "../Presentation/SwapChain.h"
#include "RenderPasses/RenderPass.h"


namespace KTXCompressor {

    class Shader {

    public:
        Shader(PhysicalDevice *physicalDevice,
               LogicalDevice *logicalDevice,
               RenderPass *renderPass,
               VkExtent2D swapChainExtent,
               const string &vertexFileName,
               const string &fragmentFileName);

        virtual ~Shader();

        virtual void Render(VkCommandBuffer vulkanCommandBuffer, uint32_t currentFrame, VkExtent2D extent) = 0;

        void Bind(VkCommandBuffer vulkanCommandBuffer, uint32_t currentFrame);

    protected:
        struct Vertex {
            glm::vec3 pos;
            glm::vec3 colour;
            glm::vec2 texCoord;
        };
        PhysicalDevice *physicalDevice;
        LogicalDevice *logicalDevice;
        BufferUtil *bufferUtil;
        VkPipelineLayout vulkanPipelineLayout = nullptr;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        vector<DescriptorSet *> descriptorSets;

        void Init(RenderPass* renderPass, VkExtent2D  swapChainExtent);

        virtual VkPipelineLayout CreatePipelineLayout() = 0;

        VkPipeline CreateVulkanPipeline(RenderPass *renderPass, VkExtent2D swapChainExtent);

        virtual void
        SetRasterizationStateCreateInfo(VkPipelineRasterizationStateCreateInfo &rasterizationStateCreateInfo) = 0;


        virtual void CreateVertexBuffer() = 0;

        virtual void CreateIndexBuffer() = 0;

        void BindDescriptorSets(VkCommandBuffer vulkanCommandBuffer, uint32_t currentFrame);

    private:
        static vector<char> ReadFile(const string &fileName);

        VkShaderModule CreateShaderModule(const string &fileName);

    public:
        virtual const char *GetVertexEntryPointName() = 0;

        virtual const char *GetFragmentEntryPointName() = 0;


    private:
        VkShaderModule vertexShaderModule;
        VkShaderModule fragmentShaderModule;
        VkPipeline shaderPipeline;

    public:
        VkShaderModule GetVertexShaderModule() {
            return vertexShaderModule;
        }

        VkShaderModule GetFragmentShaderModule() {
            return fragmentShaderModule;
        }

        VkPipelineLayout GetVulkanPipelineLayout();

        VkVertexInputBindingDescription GetBindingDescription();

        vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();

        void CleanUpShaderModules();

    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_SHADER_H
