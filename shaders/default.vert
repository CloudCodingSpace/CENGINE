// #version 450

// layout (location = 0) in vec3 position;
// layout (location = 1) in vec3 colour;

// layout (location = 3) out vec3 color;

// void main() {
//     gl_Position = vec4(position, 1.0);
//     color = colour;
// }


#version 450

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

layout (location = 0) out vec3 col;

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    col = colors[gl_VertexIndex];
}