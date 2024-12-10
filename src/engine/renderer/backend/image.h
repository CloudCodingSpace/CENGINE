#pragma once

#include <engine/utils.h>
#include "device.h"
#include "buffer.h"

#include <vulkan/vulkan.h>

typedef struct image {
    VkImage img;
    VkDeviceMemory imgMem;
    VkImageView view;
    VkFormat format;
} image;

void bcknd_create_image(image* img, 
        Device* device, 
        VkFormat format, 
        VkExtent3D extent, 
        VkImageTiling tiling, 
        VkImageUsageFlags usage, 
        VkImageAspectFlagBits flags, 
        VkMemoryPropertyFlags memFlags);
void bcknd_destroy_image(image* img, Device* device);