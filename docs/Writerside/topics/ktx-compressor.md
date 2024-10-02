# KTX-Compressor


## Requirements
1. Vcpkg: Download vcpkg - <u>https://vcpkg.io/en/</u> and <u>https://github.com/microsoft/vcpkg/releases/</u>
2. KTX-Software: ./vcpkg install ktx
3. ImageInput: ./vcpkg install openimageio 



Set up Vulkan Sdk on linux after install SDK and extrating somwhere
IN TERMINAL
1. $nano ~/.profile
2. add to bottom of file 'source /path/to/yourVULKANSDK/1.3.283.0/setup-env.sh'
3. Save the file by pressing Ctrl + X then Y and then Enter
4. RESTART COMPUTER
5. should be able to run $vulkaninfo or $vkcube in any terminal



go to src/KtxCompressor and simply run  ./vcpkg to install all required libs for project



LINUX:
GLFW3 currently requires the following libraries from the system package manager:
xinerama
xcursor
xorg
libglu1-mesa
pkg-config

These can be installed on Ubuntu systems via sudo apt install libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev pkg-config
