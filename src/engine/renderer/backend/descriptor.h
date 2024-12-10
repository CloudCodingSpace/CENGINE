#pragma once

#include <vulkan/vulkan.h>

#include <engine/utils.h>
#include "device.h"

void bcknd_create_desc_pool(VkDescriptorPool* pool, 
                            Device* device, 
                            uint32_t maxSets, 
                            uint32_t sizeCount,  
                            VkDescriptorPoolSize* pSizes);
void bcknd_destroy_desc_pool(VkDescriptorPool* pool, Device* device);

void bcknd_create_desc_layout(VkDescriptorSetLayout* layout, 
            Device* device,
            uint32_t bindingCount,
            VkDescriptorSetLayoutBinding* bindings);
void bcknd_destroy_desc_layout(VkDescriptorSetLayout* layout, Device* device);

void bcknd_create_desc_set(VkDescriptorSet* set, uint32_t count, Device* device, VkDescriptorPool pool, VkDescriptorSetLayout* layout);
void bcknd_destroy_desc_set(VkDescriptorSet* set, uint32_t count, Device* device, VkDescriptorPool pool);