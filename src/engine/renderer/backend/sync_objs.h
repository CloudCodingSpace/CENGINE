#pragma once

#include <vulkan/vulkan.h>

#include <engine/utils.h>
#include "device.h"

void bcknd_create_fence(VkFence* fence, device* device);
void bcknd_destroy_fence(VkFence* fence, device* device);

void bcknd_create_semaphore(VkSemaphore* sema, device* device);
void bcknd_destroy_semaphore(VkSemaphore* sema, device* device);