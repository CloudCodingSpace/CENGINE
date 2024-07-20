#include "sync_objs.h"

void create_fence(VkFence *fence, device *device) {
    VkFenceCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    VK_CHECK(vkCreateFence(device->logical, &info, 0, fence))
}

void destroy_fence(VkFence *fence, device *device) {
    vkDestroyFence(device->logical, *fence, 0);
}

void create_semaphore(VkSemaphore *sema, device *device) {
    VkSemaphoreCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };

    VK_CHECK(vkCreateSemaphore(device->logical, &info, 0, sema))
}

void destroy_semaphore(VkSemaphore *sema, device *device) {
    vkDestroySemaphore(device->logical, *sema, 0);
}