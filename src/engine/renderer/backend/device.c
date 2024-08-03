#include "device.h"

VkFormat find_supported_formats(VkPhysicalDevice device, VkFormat *candidates, VkImageTiling tiling, VkFormatFeatureFlags flags) {
    for(uint32_t i = 0; i < ARR_SIZE(candidates); i++) {
        VkFormat format = candidates[i];
        VkFormatProperties props = {};
        vkGetPhysicalDeviceFormatProperties(device, format, &props);
        if(tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & flags)) {
            return format;
        }
        else if(tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & flags)) {
            return format;
        }
    }
    
    return VK_FORMAT_UNDEFINED;
}

bool get_depth_format(VkPhysicalDevice device, VkFormat* format)
{
    VkFormat candidates[3] = {
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT
    };

    *format = find_supported_formats(device, candidates, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

    if(*format == VK_FORMAT_UNDEFINED) { return false; }
    return true;
}

bool is_device_extensions_supported(VkPhysicalDevice device)
{
    uint32_t extCount = 0;
    VK_CHECK(vkEnumerateDeviceExtensionProperties(device, 0, &extCount, 0))
    VkExtensionProperties* props = (VkExtensionProperties*) calloc(extCount, sizeof(VkExtensionProperties));
    VK_CHECK(vkEnumerateDeviceExtensionProperties(device, 0, &extCount, props))

    uint32_t extsFound = 0;
    uint32_t extsRequired = 1;

    for(uint32_t i = 0; i < extCount; i++) {
        if(strcmp(props[i].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
            extsFound++;
        }
    }

    free(props);
    return extsFound == extsRequired;
}

bool is_device_usable(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    queue_families families = find_queue_families(device, surface);

    physical_device_sc_caps caps = get_physical_device_caps(device, surface);
    VkFormat depthFormat;

    return is_queue_family_complete(&families) && is_device_extensions_supported(device) && ARR_SIZE(caps.modes) && ARR_SIZE(caps.formats) && get_depth_format(device, &depthFormat);
}

void select_physical_device(device* device, instance* instance, VkSurfaceKHR surface) {
    uint32_t count = 0;
    vkEnumeratePhysicalDevices(instance->instance, &count, 0);
    VkPhysicalDevice* devices = (VkPhysicalDevice*) calloc(count, sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(instance->instance, &count, devices);

    if(count == 0) {
        FATAL("Failed to find any GPU in the current PC@")
    }

    for(uint32_t i = 0; i < count; i++) {
        if(is_device_usable(devices[i], surface)) {
            device->physical = devices[i];
        }
    }
    free(devices);

    if(device->physical == VK_NULL_HANDLE) {
        FATAL("Failed to find a suitable gpu from the current PC!")
    }
    device->families = find_queue_families(device->physical, surface);
    get_depth_format(device->physical, &device->depthFormat);
}

physical_device_sc_caps get_physical_device_caps(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    physical_device_sc_caps caps;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &caps.caps);
    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, 0);
    VkSurfaceFormatKHR formats[formatCount];
    caps.formats = formats;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, caps.formats);
    uint32_t modeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, 0);
    VkPresentModeKHR modes[modeCount];
    caps.modes = modes;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, caps.modes);

    return caps;
}

bool is_queue_family_complete(queue_families *family)
{
    return (family->qIdx != INVALID_IDX);
}

queue_families find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface) {
    queue_families families;

    uint32_t qFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &qFamilyCount, 0);
    VkQueueFamilyProperties* props = (VkQueueFamilyProperties*) calloc(qFamilyCount, sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(device, &qFamilyCount, props);

    VkBool32 graphicsSupported = VK_FALSE;
    VkBool32 transferSupported = VK_FALSE;

    for(uint32_t i = 0; i < qFamilyCount; i++) {
        if(props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            families.qIdx = i;
            graphicsSupported = VK_TRUE;
        }

        VkBool32 present = VK_FALSE;
        VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present))

        if(props[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
            transferSupported = VK_TRUE;
        }

        if(present && graphicsSupported && transferSupported) {
            break;
        }
    }

    free(props);

    return families;
}

void retrive_queue_objects(device* device) {
    vkGetDeviceQueue(device->logical, device->families.qIdx, 0, &device->families.queue);
}

void create_device(device* device, instance* instance, VkSurfaceKHR surface) {
    device->families.qIdx = INVALID_IDX;

    device->physical = VK_NULL_HANDLE;

    select_physical_device(device, instance, surface);

    VkDeviceQueueCreateInfo qInfos[1];
    float qPriority = 1.0f;

    VkDeviceQueueCreateInfo qInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pQueuePriorities = &qPriority,
        .queueCount = 1,
        .queueFamilyIndex = device->families.qIdx
    };

    qInfos[0] = qInfo;

    VkPhysicalDeviceFeatures features = {0};

    const char* exts[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkDeviceCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pEnabledFeatures = &features,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = qInfos,
        .enabledExtensionCount = 1,
        .ppEnabledExtensionNames = exts
    };

#ifdef _DEBUG
    const char* layer[] = {LAYERNAME};

    info.enabledLayerCount = 1;
    info.ppEnabledLayerNames = layer;
#else
    info.enabledLayerCount = 0;
#endif

    VK_CHECK(vkCreateDevice(device->physical, &info, 0, &device->logical))
    retrive_queue_objects(device);
}

void destroy_device(device* device) {
    vkDestroyDevice(device->logical, 0);
}