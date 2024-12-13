#pragma once

#include <engine/utils.h>
#include "device.h"
#include "Buffer.h"

#include <vulkan/vulkan.h>

typedef struct {
    VkImage img;
    VkDeviceMemory imgMem;
    VkImageView view;
    VkFormat format;
} Image;

void bcknd_create_image(Image* img, 
        Device* device, 
        VkFormat format, 
        VkExtent3D extent, 
        VkImageTiling tiling, 
        VkImageUsageFlags usage, 
        VkImageAspectFlagBits flags, 
        VkMemoryPropertyFlags memFlags);
void bcknd_destroy_image(Image* img, Device* device);