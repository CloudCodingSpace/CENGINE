#pragma once

#include <engine/utils.h>
#include "device.h"
#include "cmd.h"

#include <stdint.h>
#include <stdlib.h>

#include <vulkan/vulkan.h>

typedef struct buffer {
    VkBuffer buff;
    VkDeviceMemory mem;
    VkDeviceSize size;
} buffer;

uint32_t bcknd_find_mem_type_idx(device* device, uint32_t type, VkMemoryPropertyFlags props);

void bcknd_allocate_device_memory(device* device, VkDeviceMemory* mem, VkMemoryRequirements memRequirements, VkMemoryPropertyFlags memFlags);

void bcknd_create_buffer(buffer* buffer, device* device, VkMemoryPropertyFlags memFlags, VkBufferUsageFlags usgFlags, VkDeviceSize size);
void bcknd_destroy_buffer(buffer* buffer, device* device);

void bcknd_upload_data_to_buff(buffer* buffer, device* device, void* data, VkDeviceSize dat_size);

void bcknd_cpy_buffer(buffer* dest, buffer* src, VkDeviceSize size, VkCommandPool* pool, device* device);