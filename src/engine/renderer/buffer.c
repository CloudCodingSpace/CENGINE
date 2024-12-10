#include "buffer.h"

void destroy_buffer(buffer* buff, Device* device) {
    bcknd_destroy_buffer(buff, device);
}

void create_staging_buffer(buffer* buff, Device* device, VkDeviceSize size, void* data) {
    bcknd_create_buffer(buff, device, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, size);
    bcknd_upload_data_to_buff(buff, device, data, size);
}

void create_vertex_buffer(buffer* buff, Device* device, VkDeviceSize size, void* data, VkCommandPool* pool)
{
    buffer staging;
    create_staging_buffer(&staging, device, size, data);
    bcknd_create_buffer(buff, device, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, size);
    
    bcknd_cpy_buffer(buff, &staging, size, pool, device);

    destroy_buffer(&staging, device);
}

void create_index_buffer(buffer* buff, Device* device, VkDeviceSize size, void* data, VkCommandPool* pool) {
    buffer staging;
    create_staging_buffer(&staging, device, size, data);
    bcknd_create_buffer(buff, device, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, size);
    
    bcknd_cpy_buffer(buff, &staging, size, pool, device);

    destroy_buffer(&staging, device);
}

void create_ubo(buffer* buff, Device* device, VkDeviceSize size) {
    bcknd_create_buffer(buff, device, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, size);
}