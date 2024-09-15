#version 450

layout (location = 0) in vec3 fragmentColours;
layout (location = 1) in vec2 fragmentTextureCoord;

layout (set = 1, binding = 1) uniform sampler2D texureSampler;

layout (location = 0) out vec4 outColour;

void main() {
    outColour = texture(texureSampler, fragmentTextureCoord * 2.0);
}