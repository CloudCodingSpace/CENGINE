#include "Mesh.h"

void create_Mesh(Mesh* Mesh, Device* device, VkCommandPool* pool, Vertex* vertices, uint32_t* indices, uint32_t vertexSize, uint32_t indSize) {
    Mesh->vertCount = indSize;
    VkDeviceSize vertSize = (VkDeviceSize) sizeof(vertices[0]) * vertexSize;
    VkDeviceSize indicesSize = (VkDeviceSize) sizeof(indices[0]) * indSize;

    create_vertex_buffer(&Mesh->vertexbuff, device, vertSize, vertices, pool);
    create_index_buffer(&Mesh->indBuff, device, indicesSize, indices, pool);
}

void render_Mesh(Mesh* Mesh, VkCommandBuffer* buff) {
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(*buff, 0, 1, &Mesh->vertexbuff.buff, offsets);
    vkCmdBindIndexBuffer(*buff, Mesh->indBuff.buff, 0, VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(*buff, Mesh->vertCount, 1, 0, 0, 0);
}

void destroy_Mesh(Mesh* Mesh, Device* device) {
    destroy_buffer(&Mesh->vertexbuff, device);
    destroy_buffer(&Mesh->indBuff, device);
}