#include "vertex.h"

VkVertexInputBindingDescription get_vertex_binding_desc() {
    VkVertexInputBindingDescription desc = {
        .binding = 0,
        .stride = sizeof(vertex),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    return desc;
}

void get_vertex_attrib_desc(VkVertexInputAttributeDescription* attribs, uint32_t* count) {
    *count = 2;
    
    if(attribs != 0) {
        attribs[0] = (VkVertexInputAttributeDescription){
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .location = 0,
            .offset = offsetof(vertex, pos)
        };

        attribs[1] = (VkVertexInputAttributeDescription){
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .location = 1,
            .offset = offsetof(vertex, col)
        };
    }
}