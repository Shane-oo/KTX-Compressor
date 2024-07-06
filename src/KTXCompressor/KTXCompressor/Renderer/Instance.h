//
// Created by shane on 6/07/2024.
//

#ifndef KTXCOMPRESSOR_INSTANCE_H
#define KTXCOMPRESSOR_INSTANCE_H


#include <vulkan/vulkan_core.h>

class Instance {
public:
    Instance();

    ~Instance();

private:
    VkInstance instance;

private:
    VkInstance CreateInstance();
};


#endif //KTXCOMPRESSOR_INSTANCE_H
