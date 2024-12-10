#pragma once

#include <vulkan/vulkan.h>

#include <engine/utils.h>

#include "device.h"
#include "swapchain.h"

typedef struct {
    VkRenderPass pass;
} Renderpass;

void create_renderpass(Renderpass*  pass, Device* device, Swapchain*  sc);

void begin_renderpass(Renderpass*  pass, 
            VkCommandBuffer buff, 
            VkFramebuffer frameBuff, 
            VkClearValue clear, 
            VkExtent2D extent, 
            VkOffset2D offset);
void end_renderpass(VkCommandBuffer buff);

void destroy_renderpass(Renderpass*  pass, Device* device);