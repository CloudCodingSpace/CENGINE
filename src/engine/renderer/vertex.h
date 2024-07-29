#pragma once

#include <vulkan/vulkan.h>

#include <cglm/cglm.h>

typedef struct vertex {
    vec3 pos;
    vec3 col;
} vertex;

VkVertexInputBindingDescription get_vertex_binding_desc();
VkVertexInputAttributeDescription* get_vertex_attrib_desc();