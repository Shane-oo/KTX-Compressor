# CMakeList.txt : CMake project for KTXCompressor, include source and define
# project specific logic here.

# Copy the textures folder to the build and binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# SHADERS

# Copy All Shaders to the build and binary directory
set(SHADERS_FOLDER Renderer/Graphics/Shaders)
set(SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${SHADERS_FOLDER})

# Recursively find all .spv files in the shaders directory
file(GLOB_RECURSE SPV_SHADERS ${SHADERS_DIR}/*.spv)

# Copy the .spv files to the top-level directory in the build directory
foreach(SPV_SHADER ${SPV_SHADERS})
    get_filename_component(SPV_SHADER_NAME ${SPV_SHADER} NAME)
    file(COPY ${SPV_SHADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

# END_SHADERS

# Add source to this project's executable.
add_executable(KTXCompressor "KTXCompressor.cpp" "KTXCompressor.h"
        Texture.cpp
        Texture.h
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
        Renderer/Instance/Devices/Queues/GraphicsQueue.cpp
        Renderer/Instance/Devices/Queues/GraphicsQueue.h
        Renderer/Instance/Devices/Queues/PresentQueue.cpp
        Renderer/Instance/Devices/Queues/PresentQueue.h
        Renderer/RendererConstants.cpp
        Renderer/Presentation/SwapChain.cpp
        Renderer/Presentation/SwapChain.h
        Renderer/Presentation/ImageView.cpp
        Renderer/Presentation/ImageView.h
        Renderer/Graphics/GraphicsPipeline.cpp
        Renderer/Graphics/GraphicsPipeline.h
        Renderer/Graphics/Shader.cpp
        Renderer/Graphics/Shader.h
        Renderer/Graphics/Shaders/SimpleTriangle/SimpleTriangleShader.cpp
        Renderer/Graphics/Shaders/SimpleTriangle/SimpleTriangleShader.h
        Renderer/Graphics/Shaders/SimpleTriangle/SimpleTriangleGraphicsPipeline.cpp
        Renderer/Graphics/Shaders/SimpleTriangle/SimpleTriangleGraphicsPipeline.h
        Renderer/Graphics/RenderPass.cpp
        Renderer/Graphics/RenderPass.h
        Renderer/Presentation/FrameBuffer.cpp
        Renderer/Presentation/FrameBuffer.h
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