#include "framebuffer.h"

void create_framebuffer(VkFramebuffer *out, device *device, VkImageView *view, VkExtent2D extent, renderpass *pass) {
    VkFramebufferCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .attachmentCount = ARR_SIZE(view),
        .pAttachments = view,
        .layers = 1,
        .width = extent.width,
        .height = extent.height,
        .renderPass = pass->pass
    };

    VK_CHECK(vkCreateFramebuffer(device->logical, &info, 0, out))
}

void destroy_framebuffer(VkFramebuffer *out, device *device) {
    vkDestroyFramebuffer(device->logical, *out, 0);
}