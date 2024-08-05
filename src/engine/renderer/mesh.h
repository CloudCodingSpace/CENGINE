#pragma once

#include "buffer.h"
#include "vertex.h"

#include <engine/utils.h>

#include <vulkan/vulkan.h>

typedef struct mesh {
    buffer vertexbuff;
    buffer indBuff;
    uint32_t vertCount;
} mesh;

void create_mesh(mesh* mesh, device* device, VkCommandPool* pool, vertex* vertices, uint32_t* indices, uint32_t vertexSize, uint32_t indSize);
void render_mesh(mesh* mesh, VkCommandBuffer* buff);
void destroy_mesh(mesh* mesh, device* device);