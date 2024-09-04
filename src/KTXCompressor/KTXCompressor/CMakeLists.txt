# CMakeList.txt : CMake project for KTXCompressor, include source and define
# project specific logic here.

# CopyBuffers the textures folder to the build and binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# SHADERS

# CopyBuffers All Shaders to the build and binary directory
set(SHADERS_FOLDER Renderer/Graphics)
set(SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${SHADERS_FOLDER})

# Recursively find all .spv files in the shaders directory
file(GLOB_RECURSE SPV_SHADERS ${SHADERS_DIR}/*.spv)

# CopyBuffers the .spv files to the top-level directory in the build directory
foreach(SPV_SHADER ${SPV_SHADERS})
    get_filename_component(SPV_SHADER_NAME ${SPV_SHADER} NAME)
    file(COPY ${SPV_SHADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

# END_SHADERS

# Add source to this project's executable.
add_executable(KTXCompressor "KTXCompressor.cpp" "KTXCompressor.h"
        TextureOld.cpp
        TextureOld.h
        Renderer/RendererApp.cpp
        Renderer/RendererApp.h
        Renderer/Presentation/Window.cpp
        Renderer/Presentation/Window.h
        Common.h
        Renderer/Instance/Instance.cpp
        Renderer/Instance/Instance.h
        Renderer/Instance/Debug/Debugger.cpp
        Renderer/Instance/Debug/Debugger.h
        Renderer/RendererConstants.h
        Renderer/Instance/Devices/PhysicalDevice.cpp
        Renderer/Instance/Devices/PhysicalDevice.h
        Renderer/Instance/Devices/Queues/QueueFamily.cpp
        Renderer/Instance/Devices/Queues/QueueFamily.h
        Renderer/Instance/Devices/LogicalDevice.cpp
        Renderer/Instance/Devices/LogicalDevice.h
        Renderer/Instance/Devices/Queues/Queue.cpp
        Renderer/Instance/Devices/Queues/Queue.h
        Renderer/RendererConstants.cpp
        Renderer/Presentation/SwapChain.cpp
        Renderer/Presentation/SwapChain.h
        Renderer/Presentation/ImageView.cpp
        Renderer/Presentation/ImageView.h
        Renderer/Graphics/GraphicsPipeline.cpp
        Renderer/Graphics/GraphicsPipeline.h
        Renderer/Graphics/Shader.cpp
        Renderer/Graphics/Shader.h
        Renderer/Graphics/SimpleTriangle/SimpleTriangleShader.cpp
        Renderer/Graphics/SimpleTriangle/SimpleTriangleShader.h
        Renderer/Graphics/SimpleTriangle/SimpleTriangleGraphicsPipeline.cpp
        Renderer/Graphics/SimpleTriangle/SimpleTriangleGraphicsPipeline.h
        Renderer/Graphics/RenderPass.cpp
        Renderer/Graphics/RenderPass.h
        Renderer/Presentation/FrameBuffer.cpp
        Renderer/Presentation/FrameBuffer.h
        Renderer/Commands/DrawCommand.cpp
        Renderer/Commands/DrawCommand.h
        Renderer/Synchronization/Synchronization.cpp
        Renderer/Synchronization/Synchronization.h
        Renderer/Utils/BufferUtil.cpp
        Renderer/Utils/BufferUtil.h
        Renderer/Commands/Command.cpp
        Renderer/Commands/Command.h
        Renderer/Commands/SingleTimeCommand.cpp
        Renderer/Commands/SingleTimeCommand.h
        Renderer/Graphics/DescriptorSets/DescriptorSet.cpp
        Renderer/Graphics/DescriptorSets/DescriptorSet.h
        Renderer/Graphics/DescriptorSets/ModelViewProjectionDescriptorSet.cpp
        Renderer/Graphics/DescriptorSets/ModelViewProjectionDescriptorSet.h
        Renderer/Graphics/Textures/Texture.cpp
        Renderer/Graphics/Textures/Texture.h
        Renderer/Graphics/DescriptorSets/CombinedImageSamplerDescriptorSet.cpp
        Renderer/Graphics/DescriptorSets/CombinedImageSamplerDescriptorSet.h
        Renderer/Graphics/Textures/DepthTexture.cpp
        Renderer/Graphics/Textures/DepthTexture.h
        Renderer/Graphics/Textures/KTXTexture.cpp
        Renderer/Graphics/Textures/KTXTexture.h
        Renderer/Graphics/Textures/ImageTexture.cpp
        Renderer/Graphics/Textures/ImageTexture.h
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET KTXCompressor PROPERTY CXX_STANDARD 20)
endif ()

# Find ktx
find_package(ktx CONFIG REQUIRED)
# Link Kts
target_link_libraries(KTXCompressor PRIVATE KTX::ktx)

# Find OpenImageIO
find_package(OpenImageIO CONFIG REQUIRED)
# Link OpenImageIo
target_link_libraries(KTXCompressor PRIVATE OpenImageIO::OpenImageIO)



#Find Vulkan
find_package(Vulkan REQUIRED)
# Link Vulkan
target_link_libraries(KTXCompressor PRIVATE Vulkan::Vulkan)

# Find GLFW package
find_package(glfw3 REQUIRED)
# Link GLFW Library
target_link_libraries(KTXCompressor PRIVATE glfw)

# Find GLM package
find_package(glm REQUIRED)
# Link GLM Library
target_link_directories(KTXCompressor PRIVATE glm)

##