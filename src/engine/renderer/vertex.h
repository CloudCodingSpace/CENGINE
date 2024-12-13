#pragma once

#include <vulkan/vulkan.h>

#include <cglm/cglm.h>
#include <stdlib.h>

typedef struct {
    vec3 pos;
    vec3 col;
} Vertex;

VkVertexInputBindingDescription get_vertex_binding_desc();
void get_vertex_attrib_desc(VkVertexInputAttributeDescription* descs, uint32_t* count);