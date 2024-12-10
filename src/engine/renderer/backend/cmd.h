#pragma once

#include <vulkan/vulkan.h>

#include <engine/utils.h>
#include "device.h"

void bcknd_create_cmd_pool(VkCommandPool* pool, Device* device);
void bcknd_destroy_cmd_pool(VkCommandPool* pool, Device* device);

VkCommandBuffer bcknd_create_command_buff(VkCommandPool pool, Device* device);
void bcknd_destroy_command_buff(VkCommandPool* pool, Device* device, VkCommandBuffer* buff);

void bcknd_begin_cmd_buff(Device* device, VkCommandBuffer* buff);
void bcknd_end_cmd_buff(VkCommandBuffer* buff);