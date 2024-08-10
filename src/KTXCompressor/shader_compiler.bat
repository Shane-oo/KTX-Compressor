@echo off
setlocal enabledelayedexpansion

:: Define the path to the glslc compiler
::set GLSLC=C:\VulkanSDK\1.3.275.0\Bin\glslc.exe
set GLSLC=C:\Program Files\VulkanSDK\1.3.283.0\Bin\glslc.exe

:: Define the root folder for all shaders
set SHADERS_DIR=KTXCompressor\Renderer\Graphics

:: Recursively find all .frag and .vert files in the SHADERS_DIR
for /r "%SHADERS_DIR%" %%f in (*.frag *.vert) do (
    :: Determine the shader stage based on the file extension
    set STAGE=
    if "%%~xf"==".vert" set STAGE=vertex
    if "%%~xf"==".frag" set STAGE=fragment

    if defined STAGE (
        echo Compiling %%f as !STAGE! shader
        "%GLSLC%" -fshader-stage=!STAGE! %%f -o %%f.spv
    ) else (
        echo Skipping %%f: Unknown shader stage
    )
)
