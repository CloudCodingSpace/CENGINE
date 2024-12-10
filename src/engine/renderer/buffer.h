#pragma once

#include "backend/buffer.h"
#include "backend/device.h"

#include <vulkan/vulkan.h>

void destroy_buffer(buffer* buff, Device* device);

void create_staging_buffer(buffer* buff, Device* device, VkDeviceSize size, void* data);
void create_vertex_buffer(buffer* buff, Device* device, VkDeviceSize size, void* data, VkCommandPool* pool);
void create_index_buffer(buffer* buff, Device* device, VkDeviceSize size, void* data, VkCommandPool* pool);
void create_ubo(buffer* buff, Device* device, VkDeviceSize size);