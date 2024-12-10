#include "cmd.h"

void bcknd_create_cmd_pool(VkCommandPool *pool, Device *device) {
    VkCommandPoolCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = device->families.qIdx
    };

    VK_CHECK(vkCreateCommandPool(device->logical, &info, 0, pool))
}

void bcknd_destroy_cmd_pool(VkCommandPool *pool, Device *device) {
    vkDestroyCommandPool(device->logical, *pool, 0);
}

VkCommandBuffer bcknd_create_command_buff(VkCommandPool pool, Device* device) {
    VkCommandBuffer buff;

    VkCommandBufferAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandPool = pool,
        .commandBufferCount = 1
    };

    VK_CHECK(vkAllocateCommandBuffers(device->logical, &allocInfo, &buff))
    return buff;
}

void bcknd_destroy_command_buff(VkCommandPool* pool, Device* device, VkCommandBuffer* buff) {
    vkFreeCommandBuffers(device->logical, *pool, 1, buff);
}

void bcknd_begin_cmd_buff(Device* device, VkCommandBuffer* buff) {
    VkCommandBufferBeginInfo begin = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
    };

    VK_CHECK(vkBeginCommandBuffer(*buff, &begin))
}

void bcknd_end_cmd_buff(VkCommandBuffer* buff) {
    VK_CHECK(vkEndCommandBuffer(*buff))
}