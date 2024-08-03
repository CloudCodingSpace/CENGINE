#pragma once

#include "buffer.h"
#include "vertex.h"

#include <engine/utils.h>

#include <vulkan/vulkan.h>

typedef struct mesh {
    buffer vertexbuff;
    uint32_t vertCount;
} mesh;

void create_mesh(mesh* mesh, device* device, VkCommandPool* pool, vertex* vertices, uint32_t vertCount);
void render_mesh(mesh* mesh, VkCommandBuffer* buff);
void destroy_mesh(mesh* mesh, device* device);