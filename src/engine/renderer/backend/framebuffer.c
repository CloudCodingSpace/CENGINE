#include "frameBuffer.h"

void create_framebuffer(VkFramebuffer *out, Device *device, VkImageView *view, uint32_t count, VkExtent2D extent, Renderpass* pass) {
    VkFramebufferCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .attachmentCount = count,
        .pAttachments = view,
        .layers = 1,
        .width = extent.width,
        .height = extent.height,
        .renderPass = pass->pass
    };

    VK_CHECK(vkCreateFramebuffer(device->logical, &info, 0, out))
}

void destroy_framebuffer(VkFramebuffer *out, Device *device) {
    vkDestroyFramebuffer(device->logical, *out, 0);
}