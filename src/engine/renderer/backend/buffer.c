#include "buffer.h"

uint32_t bcknd_find_mem_type_idx(Device* device, uint32_t type, VkMemoryPropertyFlags props) {
    VkPhysicalDeviceMemoryProperties devProps;
    vkGetPhysicalDeviceMemoryProperties(device->physical, &devProps);

    for(uint32_t i = 0; i < devProps.memoryTypeCount; i++) {
        if ((type & (1 << i)) && (devProps.memoryTypes[i].propertyFlags & props) == props) {
            return i; 
        }
    }
    
    return INVALID_IDX;
}

void bcknd_allocate_device_memory(Device* device, VkDeviceMemory* mem, VkMemoryRequirements memRequirements, VkMemoryPropertyFlags memFlags) {
        
        uint32_t type = bcknd_find_mem_type_idx(device, memRequirements.memoryTypeBits, memFlags);
        
        if(type == INVALID_IDX)
            FATAL("Failed to find suitable memory type index!")

        VkMemoryAllocateInfo info = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .memoryTypeIndex = type,
            .allocationSize = memRequirements.size
        };

        VK_CHECK(vkAllocateMemory(device->logical, &info, 0, mem))
}

void bcknd_create_buffer(buffer *buffer, Device *device, VkMemoryPropertyFlags memFlags, VkBufferUsageFlags usgFlags, VkDeviceSize size)
{
    buffer->size = size;
    
    // Buff
    VkBufferCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .usage = usgFlags,
        .size = size
    };

    VK_CHECK(vkCreateBuffer(device->logical, &info, 0, &buffer->buff))

    // Allocating memory
    VkMemoryRequirements memRequirements = {};
    vkGetBufferMemoryRequirements(device->logical, buffer->buff, &memRequirements);
    bcknd_allocate_device_memory(device, &buffer->mem, memRequirements, memFlags);

    VK_CHECK(vkBindBufferMemory(device->logical, buffer->buff, buffer->mem, 0))
}

void bcknd_destroy_buffer(buffer* buffer, Device* device) {
    vkDestroyBuffer(device->logical, buffer->buff, 0);
    vkFreeMemory(device->logical, buffer->mem, 0);
}

void bcknd_upload_data_to_buff(buffer* buffer, Device* device, void* data, VkDeviceSize dat_size) {
    void* dat;
    vkMapMemory(device->logical, buffer->mem, 0, buffer->size, 0, &dat);
    memcpy(dat, data, (size_t) dat_size);
    vkUnmapMemory(device->logical, buffer->mem);
}

void bcknd_cpy_buffer(buffer* dest, buffer* src, VkDeviceSize size, VkCommandPool* pool, Device* device) {
    VkCommandBuffer buff = bcknd_create_command_buff(*pool, device);
    bcknd_begin_cmd_buff(device, &buff);

    VkBufferCopy region = {
        .size = size
    };
    vkCmdCopyBuffer(buff, src->buff, dest->buff, 1, &region);

    bcknd_end_cmd_buff(&buff);

    VkSubmitInfo submit = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &buff
    };
    VK_CHECK(vkQueueSubmit(device->families.queue, 1, &submit, VK_NULL_HANDLE))
    vkQueueWaitIdle(device->families.queue);

    bcknd_destroy_command_buff(pool, device, &buff);
}