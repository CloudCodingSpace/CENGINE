#pragma once

#include <engine/utils.h>
#include <vulkan/vulkan.h>

#include "device.h"
#include "renderpass.h"

void create_framebuffer(VkFramebuffer *out, device *device, VkImageView *view, VkExtent2D extent, renderpass *pass);
void destroy_framebuffer(VkFramebuffer *out, device *device);