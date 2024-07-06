//
// Created by shane on 6/07/2024.
//

#include "Instance.h"

// #region Private Methods

VkInstance Instance::CreateInstance() {
    VkApplicationInfo applicationInfo{};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = "KTX-Compressor";
    applicationInfo.applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
    applicationInfo.pEngineName = "No Engine";
    applicationInfo.engineVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
    applicationInfo.apiVersion = VK_API_VERSION_1_3;
    // todo
    return nullptr;
}

// #region Constructors

Instance::Instance() {
    instance = CreateInstance();
}

// #endregion 

// #region Destructors

Instance::~Instance() {

}


// #endregion