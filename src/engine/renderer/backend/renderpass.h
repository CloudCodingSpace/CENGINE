#pragma once

#include <vulkan/vulkan.h>

#include <engine/utils.h>

#include "device.h"
#include "swapchain.h"

typedef struct renderpass {
    VkRenderPass pass;
} renderpass;

void create_renderpass(renderpass* pass, device* device, swapchain* sc);
void destroy_renderpass(renderpass* pass, device* device);