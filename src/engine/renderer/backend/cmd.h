#pragma once

#include <vulkan/vulkan.h>

#include <engine/utils.h>
#include "device.h"

void bcknd_create_cmd_pool(VkCommandPool* pool, device* device);
void bcknd_destroy_cmd_pool(VkCommandPool* pool, device* device);

VkCommandBuffer bcknd_create_command_buff(VkCommandPool pool, device* device);
void bcknd_destroy_command_buff(VkCommandPool* pool, device* device, VkCommandBuffer* buff);

void bcknd_begin_cmd_buff(device* device, VkCommandBuffer* buff);
void bcknd_end_cmd_buff(VkCommandBuffer* buff);