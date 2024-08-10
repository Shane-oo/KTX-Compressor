//
// Created by shane on 28/07/2024.
//

#ifndef KTXCOMPRESSOR_SHADER_H
#define KTXCOMPRESSOR_SHADER_H

#include <vulkan/vulkan_core.h>
#include <glm/glm.hpp>
#include "../../Common.h"
#include "../Instance/Devices/PhysicalDevice.h"


namespace KTXCompressor {

    class Shader {

    public:
        Shader(PhysicalDevice *physicalDevice, VkDevice device, const string &vertexFileName,
               const string &fragmentFileName);

        virtual ~Shader();

        virtual void Render(VkCommandBuffer vulkanCommandBuffer) = 0;

    protected:
        PhysicalDevice *physicalDevice;
        VkDevice vulkanDevice;
        VkPipelineLayout vulkanPipelineLayout = nullptr;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;

        virtual VkPipelineLayout CreatePipelineLayout() = 0;

        void Init();

        virtual VkBuffer CreateVertexBuffer() = 0;

        virtual void FillVertexBuffer() = 0;

        struct Vertex {
            glm::vec2 pos;
            glm::vec3 colour;
        };

        VkDeviceMemory AllocateBufferMemory(VkBuffer);

    private:


        static vector<char> ReadFile(const string &fileName);

        VkShaderModule CreateShaderModule(const string &fileName);

    public:
        virtual const char *GetVertexEntryPointName() = 0;

        virtual const char *GetFragmentEntryPointName() = 0;


    private:
        VkShaderModule vertexShaderModule;
        VkShaderModule fragmentShaderModule;

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

        void Bind(VkCommandBuffer vulkanCommandBuffer);
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_SHADER_H
