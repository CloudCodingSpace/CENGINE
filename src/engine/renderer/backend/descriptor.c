#include "descriptor.h"

void bcknd_create_desc_pool(VkDescriptorPool* pool, 
                            Device* device, 
                            uint32_t maxSets, 
                            uint32_t sizeCount,  
                            VkDescriptorPoolSize* pSizes) {    
    VkDescriptorPoolCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = maxSets,
        .poolSizeCount = sizeCount,
        .pPoolSizes = pSizes
    };

    VK_CHECK(vkCreateDescriptorPool(device->logical, &info, 0, pool))
}

void bcknd_destroy_desc_pool(VkDescriptorPool* pool, Device* device) {
    vkDestroyDescriptorPool(device->logical, *pool, 0);
}

void bcknd_create_desc_layout(VkDescriptorSetLayout* layout, 
            Device* device,
            uint32_t bindingCount,
            VkDescriptorSetLayoutBinding* bindings) {

    VkDescriptorSetLayoutCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = bindingCount,
        .pBindings = bindings
    };

    VK_CHECK(vkCreateDescriptorSetLayout(device->logical, &info, 0, layout))
}

void bcknd_destroy_desc_layout(VkDescriptorSetLayout* layout, Device* device) {
    vkDestroyDescriptorSetLayout(device->logical, *layout, 0);
}

void bcknd_create_desc_set(VkDescriptorSet* set, uint32_t count, Device* device, VkDescriptorPool pool, VkDescriptorSetLayout* layout) { 
    VkDescriptorSetAllocateInfo info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorSetCount = count,
        .descriptorPool = pool,
        .pSetLayouts = layout
    };

    VK_CHECK(vkAllocateDescriptorSets(device->logical, &info, set))
}

void bcknd_destroy_desc_set(VkDescriptorSet* set, uint32_t count, Device* device, VkDescriptorPool pool) {
    VK_CHECK(vkFreeDescriptorSets(device->logical, pool, count, set))
}