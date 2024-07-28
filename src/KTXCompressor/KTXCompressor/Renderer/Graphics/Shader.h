//
// Created by shane on 28/07/2024.
//

#ifndef KTXCOMPRESSOR_SHADER_H
#define KTXCOMPRESSOR_SHADER_H

#include <vulkan/vulkan_core.h>
#include "../../Common.h"

namespace KTXCompressor {

    class Shader {
    public:
        Shader(VkDevice device, const string &vertexFileName, const string &fragmentFileName);

        ~Shader();
        
    private:
        static vector<char> ReadFile(const string &fileName);

        VkShaderModule CreateShaderModule(const string &fileName);
        
    public:
        virtual const char * GetVertexEntryPointName() = 0;
        virtual const char * GetFragmentEntryPointName() = 0;

    private:
        VkDevice vulkanDevice;
        VkShaderModule vertexShaderModule;
        VkShaderModule fragmentShaderModule;
    public:
        VkShaderModule GetVertexShaderModule() {
            return vertexShaderModule;
        }

        VkShaderModule GetFragmentShaderModule() {
            return fragmentShaderModule;
        }
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_SHADER_H
