#pragma once

#include <vulkan/vulkan.h>

#include <engine/utils.h>

#include "device.h"
#include "swapchain.h"

typedef struct renderpass {
    VkRenderPass pass;
} renderpass;

void create_renderpass(renderpass* pass, device* device, swapchain* sc);

void begin_renderpass(renderpass *pass, 
            VkCommandBuffer buff, 
            VkFramebuffer frameBuff, 
            VkClearValue clear, 
            VkExtent2D extent, 
            VkOffset2D offset);
void end_renderpass(VkCommandBuffer buff);

void destroy_renderpass(renderpass* pass, device* device);