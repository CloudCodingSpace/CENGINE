#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colour;

layout (location = 3) out vec3 color;

layout(set = 0, binding = 0) uniform global_ubo_data {
    mat4 proj;
    mat4 view;
    mat4 model;
} ubo;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(position, 1.0);
    color = colour;
}