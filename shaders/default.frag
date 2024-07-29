// #version 450

// layout (location = 0) out vec4 fragColor;

// layout (location = 3) in vec3 color;

// void main()
// {
//     fragColor = vec4(color, 1.0f);
// }

#version 450

layout (location = 0) in vec3 col;

layout (location = 0) out vec4 outColor;


void main() {
    outColor = vec4(col, 1.0);
}