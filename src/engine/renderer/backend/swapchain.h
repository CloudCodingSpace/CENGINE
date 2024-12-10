#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <engine/utils.h>
#include "device.h"
#include "surface.h"
#include "image.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    VkSwapchainKHR swapchain;
    uint32_t imgCount;
    VkImage* imgs;
    VkImageView* imgViews;
    image depthImg;

    VkSurfaceFormatKHR format;
    VkPresentModeKHR mode;
    VkExtent2D extent;

    physical_device_sc_caps caps;
} Swapchain;

void get_swapchain_imgs(Swapchain*  sc, Device* device);
void create_swapchain_img_views(Swapchain*  sc, Device* device);
void destroy_swapchain_img_views(Swapchain*  sc, Device* device);

void select_sc_format(Swapchain* sc);
void select_sc_mode(Swapchain* sc);
void select_sc_extent(Swapchain* sc, GLFWwindow* window);

void create_swapchain(Swapchain* sc, Instance* inst, Device* device, Surface* surface, GLFWwindow* win_ptr);
void destroy_swapchain(Swapchain* sc, Device* device);