//
// Created by shane on 28/07/2024.
//

#include <fstream>
#include "Shader.h"

namespace KTXCompressor {

    // #region Private Methods

    vector<char> Shader::ReadFile(const string &fileName) {
        ifstream file(fileName, ios::ate | ios::binary);

        if (!file.is_open()) {
            throw runtime_error("Failed to Open File: " + fileName);
        }

        streamsize fileSize = file.tellg();
        vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    VkShaderModule Shader::CreateShaderModule(const string &fileName) {
        auto code = ReadFile(fileName);

        VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
        shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleCreateInfo.codeSize = code.size();
        shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

        VkShaderModule shaderModule;
        VkResult createShaderModuleResult = vkCreateShaderModule(vulkanDevice,
                                                                 &shaderModuleCreateInfo,
                                                                 nullptr,
                                                                 &shaderModule);
        if (createShaderModuleResult != VK_SUCCESS) {
            throw runtime_error("Failed to Create Shader Module for " + fileName);
        }

        cout << "Successfully Created Shader Module for " << fileName << endl;

        return shaderModule;
    }

    // #endregion


    // #region Constructors

    Shader::Shader(VkDevice vulkanDevice, const string &vertexFileName, const string &fragmentFileName) {
        this->vulkanDevice = vulkanDevice;

        vertexShaderModule = CreateShaderModule(vertexFileName);
        fragmentShaderModule = CreateShaderModule(fragmentFileName);
        
        // ...
    }

    // #endregion

    // #region Destructors 

    Shader::~Shader() {
        cout << "Destroy Shader" << endl;

        vkDestroyShaderModule(vulkanDevice, fragmentShaderModule, nullptr);
        vkDestroyShaderModule(vulkanDevice, vertexShaderModule, nullptr);
    }


    // #endregion

} // KTXCompressor