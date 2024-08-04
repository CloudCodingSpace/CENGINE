#include "image.h"

void bcknd_create_image(image* img, 
        device* device, 
        VkFormat format, 
        VkExtent3D extent, 
        VkImageTiling tiling, 
        VkImageUsageFlags usage, 
        VkImageAspectFlagBits flags, 
        VkMemoryPropertyFlags memFlags) {

    img->format = format;
    
    // Image
    {
        VkImageCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .extent = extent,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .format = format,
            .imageType = VK_IMAGE_TYPE_2D,
            .arrayLayers = 1,
            .mipLevels = 1,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .tiling = tiling,
            .usage = usage
        };

        VK_CHECK(vkCreateImage(device->logical, &info, 0, &img->img));
    }
    // Image Memory
    {
        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device->logical, img->img, &memRequirements);
        bcknd_allocate_device_memory(device, &img->imgMem, memRequirements, memFlags);

        VK_CHECK(vkBindImageMemory(device->logical, img->img, img->imgMem, 0))
    }
    // Image View
    {
        VkImageViewCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .format = format,
            .image = img->img,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .subresourceRange = {
                .aspectMask = flags,
                .baseArrayLayer = 0,
                .baseMipLevel = 0,
                .layerCount = 1,
                .levelCount = 1
            }
        };

        VK_CHECK(vkCreateImageView(device->logical, &info, 0, &img->view))
    }
}

void bcknd_destroy_image(image* img, device* device) {
    vkDestroyImage(device->logical, img->img, 0);
    vkDestroyImageView(device->logical, img->view, 0);
    vkFreeMemory(device->logical, img->imgMem, 0);
}