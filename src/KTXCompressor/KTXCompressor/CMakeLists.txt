# CMakeList.txt : CMake project for KTXCompressor, include source and define
# project specific logic here.

# Copy the textures folder to the build and binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

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
        Renderer/Instance/Debugger.cpp
        Renderer/Instance/Debugger.h
        Renderer/RendererConstants.h
        Renderer/Instance/PhysicalDevice/PhysicalDevice.cpp
        Renderer/Instance/PhysicalDevice/PhysicalDevice.h
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