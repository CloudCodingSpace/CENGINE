#include "swapchain.h"

void get_swapchain_imgs(swapchain* sc, device* device) {
    sc->imgCount = 0;
    VK_CHECK(vkGetSwapchainImagesKHR(device->logical, sc->swapchain, &sc->imgCount, 0))
    sc->imgs = (VkImage*) calloc(sc->imgCount, sizeof(VkImage));
    VK_CHECK(vkGetSwapchainImagesKHR(device->logical, sc->swapchain, &sc->imgCount, sc->imgs))
}

void create_swapchain_img_views(swapchain* sc, device* device) {
    sc->imgViews = (VkImageView*) calloc(sc->imgCount, sizeof(VkImageView));

    for(uint32_t i = 0; i < sc->imgCount; i++) {
        VkImageViewCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .components = {
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY
                },
            .format = sc->format.format,
            .subresourceRange.levelCount = 1,
            .subresourceRange.layerCount = 1,
            .subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .subresourceRange.baseMipLevel = 0,
            .subresourceRange.baseArrayLayer = 0,
            .image = sc->imgs[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D
        };

        VK_CHECK(vkCreateImageView(device->logical, &info, 0, &sc->imgViews[i]))
    }
}

void destroy_swapchain_img_views(swapchain* sc, device* device) {
    for(uint32_t i = 0; i < sc->imgCount; i++) {
        vkDestroyImageView(device->logical, sc->imgViews[i], 0);
    }

    free(sc->imgViews);
}

void select_sc_format(swapchain* sc) {
    for(uint32_t i = 0; i < sc->caps.formatCount; i++) {
        if((sc->caps.formats[i].format == VK_FORMAT_B8G8R8A8_SRGB) && (sc->caps.formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)) {
            sc->format = sc->caps.formats[i];
            return;
        }
    }

    sc->format = sc->caps.formats[0];
}

void select_sc_mode(swapchain* sc) {    
    for(uint32_t i = 0; i < sc->caps.modeCount; i++) {
        if(sc->caps.modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            sc->mode = sc->caps.modes[i];
        }
    }

    sc->mode = VK_PRESENT_MODE_FIFO_KHR;
}

void select_sc_extent(swapchain* sc, GLFWwindow* window) {
    if(sc->caps.caps.currentExtent.width != UINT32_MAX) {
        sc->extent = sc->caps.caps.currentExtent;
    } 
    else {
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        VkExtent2D actualExtent = {
            (uint32_t) width,
            (uint32_t) height
        };

        actualExtent.width = clamp_uint(actualExtent.width, sc->caps.caps.minImageExtent.width, sc->caps.caps.maxImageExtent.width);
        actualExtent.height = clamp_uint(actualExtent.height, sc->caps.caps.minImageExtent.height, sc->caps.caps.maxImageExtent.height);
    
        sc->extent = actualExtent;
    }
}

void create_swapchain(swapchain *sc, instance *inst, device *device, win_surface *surface, GLFWwindow* win_ptr)
{
    sc->caps = get_physical_device_caps(device->physical, surface->surface);
    select_sc_mode(sc);
    select_sc_extent(sc, win_ptr);
    select_sc_format(sc);

    uint32_t imgCount = sc->caps.caps.minImageCount + 1;
    if(sc->caps.caps.maxImageCount > 0 && imgCount > sc->caps.caps.maxImageCount) {
        imgCount = sc->caps.caps.maxImageCount;
    }

    VkSwapchainCreateInfoKHR info = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface->surface,
        .minImageCount = imgCount,
        .imageFormat = sc->format.format,
        .imageExtent = sc->extent,
        .presentMode = sc->mode,
        .imageColorSpace = sc->format.colorSpace,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = sc->caps.caps.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };

    VK_CHECK(vkCreateSwapchainKHR(device->logical, &info, 0, &sc->swapchain))

    get_swapchain_imgs(sc, device);
    create_swapchain_img_views(sc, device);

    // Creating the depth image
    bcknd_create_image(&sc->depthImg, 
                    device, 
                    device->depthFormat, 
                    (VkExtent3D){sc->extent.width, sc->extent.height, 1}, 
                    VK_IMAGE_TILING_OPTIMAL, 
                    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
                    VK_IMAGE_ASPECT_DEPTH_BIT, 
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
}

void destroy_swapchain(swapchain* sc, device* device) {
    bcknd_destroy_image(&sc->depthImg, device);
    destroy_swapchain_img_views(sc, device);
    free(sc->imgs);
    vkDestroySwapchainKHR(device->logical, sc->swapchain, 0);
}