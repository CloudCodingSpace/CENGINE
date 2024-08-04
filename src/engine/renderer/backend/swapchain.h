#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <engine/utils.h>
#include "device.h"
#include "surface.h"
#include "image.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct swapchain {
    VkSwapchainKHR swapchain;
    uint32_t imgCount;
    VkImage* imgs;
    VkImageView* imgViews;
    image depthImg;

    VkSurfaceFormatKHR format;
    VkPresentModeKHR mode;
    VkExtent2D extent;

    physical_device_sc_caps caps;
} swapchain;

void get_swapchain_imgs(swapchain* sc, device* device);
void create_swapchain_img_views(swapchain* sc, device* device);
void destroy_swapchain_img_views(swapchain* sc, device* device);

void select_sc_format(swapchain* sc);
void select_sc_mode(swapchain* sc);
void select_sc_extent(swapchain* sc, GLFWwindow* window);

void create_swapchain(swapchain* sc, instance* inst, device* device, win_surface* surface, GLFWwindow* win_ptr);
void destroy_swapchain(swapchain* sc, device* device);