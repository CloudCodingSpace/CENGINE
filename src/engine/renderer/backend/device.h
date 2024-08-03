#pragma once

#include <vulkan/vulkan.h>

#include <engine/logger.h>
#include <engine/utils.h>
#include "instance.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct queue_families {
    uint32_t qIdx;

    VkQueue queue;
} queue_families;

typedef struct device {
    VkFormat depthFormat;
    VkPhysicalDevice physical;
    VkDevice logical;

    queue_families families;
} device;

typedef struct physical_device_sc_caps {
    VkSurfaceCapabilitiesKHR caps;
    VkPresentModeKHR* modes;
    VkSurfaceFormatKHR* formats;
} physical_device_sc_caps;

VkFormat find_supported_formats(VkPhysicalDevice device, VkFormat* candidates, VkImageTiling tiling, VkFormatFeatureFlags flags);
bool get_depth_format(VkPhysicalDevice device, VkFormat* format);

bool is_device_extensions_supported(VkPhysicalDevice device);

bool is_device_usable(VkPhysicalDevice device, VkSurfaceKHR surface);
void select_physical_device(device* device, instance* instance, VkSurfaceKHR surface);

physical_device_sc_caps get_physical_device_caps(VkPhysicalDevice device, VkSurfaceKHR surface);

bool is_queue_family_complete(queue_families* family);
queue_families find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface);
void retrive_queue_objects(device* device);

void create_device(device* device, instance* instance, VkSurfaceKHR surface);
void destroy_device(device* device);