#pragma once

#include "backend/buffer.h"
#include "backend/device.h"

#include <vulkan/vulkan.h>

void destroy_buffer(Buffer* buff, Device* device);

void create_staging_buffer(Buffer* buff, Device* device, VkDeviceSize size, void* data);
void create_vertex_buffer(Buffer* buff, Device* device, VkDeviceSize size, void* data, VkCommandPool* pool);
void create_index_buffer(Buffer* buff, Device* device, VkDeviceSize size, void* data, VkCommandPool* pool);
void create_ubo(Buffer* buff, Device* device, VkDeviceSize size);