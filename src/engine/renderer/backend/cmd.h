#pragma once

#include <vulkan/vulkan.h>

#include <engine/utils.h>
#include "device.h"

void create_cmd_pool(VkCommandPool* pool, device* device);
void destroy_cmd_pool(VkCommandPool* pool, device* device);

VkCommandBuffer create_command_buff(VkCommandPool pool, device* device);
void destroy_command_buff(VkCommandPool* pool, device* device, VkCommandBuffer* buff);

void begin_cmd_buff(device* device, VkCommandBuffer* buff);
void end_cmd_buff(VkCommandBuffer* buff);