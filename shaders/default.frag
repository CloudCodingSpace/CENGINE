#version 450

layout (location = 0) out vec4 fragColor;

layout (location = 3) in vec3 color;
layout (location = 4) in vec2 coord;

layout (binding = 1) uniform sampler2D tex;

void main()
{
    fragColor = texture(tex, coord);
}