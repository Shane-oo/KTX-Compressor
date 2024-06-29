# CMakeList.txt : CMake project for KTXCompressor, include source and define
# project specific logic here.

# Copy the textures folder to the build and binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Add source to this project's executable.
add_executable (KTXCompressor "KTXCompressor.cpp" "KTXCompressor.h"
        Texture.cpp
        Texture.h)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET KTXCompressor PROPERTY CXX_STANDARD 20)
endif()

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