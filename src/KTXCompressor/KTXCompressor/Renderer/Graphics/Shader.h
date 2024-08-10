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
        struct Vertex {
            glm::vec2 pos;
            glm::vec3 colour;
        };
        PhysicalDevice *physicalDevice;
        VkDevice vulkanDevice;
        VkPipelineLayout vulkanPipelineLayout = nullptr;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;

        void Init();

       
        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        virtual VkPipelineLayout CreatePipelineLayout() = 0;


        virtual void CreateVertexBuffer() = 0;

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
