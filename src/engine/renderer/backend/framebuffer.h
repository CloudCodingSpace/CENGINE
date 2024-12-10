#pragma once

#include <engine/utils.h>
#include <vulkan/vulkan.h>

#include "device.h"
#include "renderpass.h"

void create_framebuffer(VkFramebuffer *out, Device *device, VkImageView *view, uint32_t count, VkExtent2D extent, Renderpass* pass);
void destroy_framebuffer(VkFramebuffer *out, Device *device);