#pragma once

#include "Buffer.h"
#include "vertex.h"

#include <engine/utils.h>

#include <vulkan/vulkan.h>

typedef struct {
    Buffer vertexbuff;
    Buffer indBuff;
    uint32_t vertCount;
} Mesh;

void create_Mesh(Mesh* Mesh, Device* device, VkCommandPool* pool, Vertex* vertices, uint32_t* indices, uint32_t vertexSize, uint32_t indSize);
void render_Mesh(Mesh* Mesh, VkCommandBuffer* buff);
void destroy_Mesh(Mesh* Mesh, Device* device);