#pragma once

#include <vulkan/vulkan.h>

#include <engine/utils.h>
#include "device.h"

void create_fence(VkFence* fence, device* device);
void destroy_fence(VkFence* fence, device* device);

void create_semaphore(VkSemaphore* sema, device* device);
void destroy_semaphore(VkSemaphore* sema, device* device);