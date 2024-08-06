#version 450

layout(location =0) in vec3 fragmentColours;

layout(location = 0) out vec4 outColour;

void main(){
    outColour = vec4(fragmentColours, 1.0);
}