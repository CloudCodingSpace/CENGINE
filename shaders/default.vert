#version 450

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 colour;
layout (location = 2) in vec2 texCoord;

layout (location = 3) out vec3 color;
layout (location = 4) out vec2 coord;

layout (set = 0, binding = 0) uniform UBO {
    mat4 model;
    mat4 proj;
    mat4 view;
} ubo;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(position, 0.0f, 1.0);
    color = colour;
    coord = texCoord;
}