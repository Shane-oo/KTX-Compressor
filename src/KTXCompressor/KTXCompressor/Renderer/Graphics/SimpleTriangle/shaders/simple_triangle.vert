#version 450

layout (set = 0, binding = 0) uniform ModelViewProjection {
    mat4 model;
    mat4 view;
    mat4 proj;
} modelViewProjectionUbo;

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec3 inColour;
layout (location = 2) in vec2 inTextureCoord;

layout (location = 0) out vec3 fragmentColours;
layout (location = 1) out vec2 fragmentTextureCoord;

void main() {
    gl_Position = modelViewProjectionUbo.proj * modelViewProjectionUbo.view * (modelViewProjectionUbo.model * vec4(inPosition, 0.0, 1.0));
    fragmentColours = inColour;
    fragmentTextureCoord = inTextureCoord;
}