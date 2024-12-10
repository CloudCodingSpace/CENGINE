#pragma once

#include <vulkan/vulkan.h>

#include <engine/logger.h>
#include <engine/utils.h>
#include "instance.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    uint32_t qIdx;

    VkQueue queue;
} QueueFamilies;

typedef struct {
    VkFormat depthFormat;
    VkPhysicalDevice physical;
    VkDevice logical;

    QueueFamilies families;
} Device;

typedef struct physical_device_sc_caps {
    VkSurfaceCapabilitiesKHR caps;
    VkPresentModeKHR* modes;
    VkSurfaceFormatKHR* formats;
    uint32_t modeCount;
    uint32_t formatCount;
} physical_device_sc_caps;

VkFormat find_supported_formats(VkPhysicalDevice device, VkFormat* candidates, uint32_t count, VkImageTiling tiling, VkFormatFeatureFlags flags);
bool get_depth_format(VkPhysicalDevice device, VkFormat* format);

bool is_device_extensions_supported(VkPhysicalDevice device);

bool is_device_usable(VkPhysicalDevice device, VkSurfaceKHR surface);
void select_physical_device(Device* device, Instance* instance, VkSurfaceKHR surface);

physical_device_sc_caps get_physical_device_caps(VkPhysicalDevice device, VkSurfaceKHR surface);

bool is_queue_family_complete(QueueFamilies* family);
QueueFamilies find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface);
void retrive_queue_objects(Device* device);

void create_device(Device* device, Instance* instance, VkSurfaceKHR surface);
void destroy_device(Device* device);