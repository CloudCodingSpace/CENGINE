#include "mesh.h"

void create_mesh(mesh* mesh, device* device, VkCommandPool* pool, vertex* vertices, uint32_t vertCount) {
    mesh->vertCount = vertCount;
    VkDeviceSize size = (VkDeviceSize) sizeof(vertices[0]) * vertCount;

    create_vertex_buffer(&mesh->vertexbuff, device, size, vertices, pool);
}

void render_mesh(mesh* mesh, VkCommandBuffer* buff) {
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(*buff, 0, 1, &mesh->vertexbuff.buff, offsets);

    vkCmdDraw(*buff, mesh->vertCount, 1, 0, 0);
}

void destroy_mesh(mesh* mesh, device* device) {
    destroy_buffer(&mesh->vertexbuff, device);
}