#include "mesh.h"

void create_mesh(mesh* mesh, Device* device, VkCommandPool* pool, vertex* vertices, uint32_t* indices, uint32_t vertexSize, uint32_t indSize) {
    mesh->vertCount = indSize;
    VkDeviceSize vertSize = (VkDeviceSize) sizeof(vertices[0]) * vertexSize;
    VkDeviceSize indicesSize = (VkDeviceSize) sizeof(indices[0]) * indSize;

    create_vertex_buffer(&mesh->vertexbuff, device, vertSize, vertices, pool);
    create_index_buffer(&mesh->indBuff, device, indicesSize, indices, pool);
}

void render_mesh(mesh* mesh, VkCommandBuffer* buff) {
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(*buff, 0, 1, &mesh->vertexbuff.buff, offsets);
    vkCmdBindIndexBuffer(*buff, mesh->indBuff.buff, 0, VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(*buff, mesh->vertCount, 1, 0, 0, 0);
}

void destroy_mesh(mesh* mesh, Device* device) {
    destroy_buffer(&mesh->vertexbuff, device);
    destroy_buffer(&mesh->indBuff, device);
}