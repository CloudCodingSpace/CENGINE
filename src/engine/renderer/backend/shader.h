#pragma once

#include <vulkan/vulkan.h>

#include <engine/utils.h>
#include "device.h"
#include "renderpass.h"

#include "../vertex.h"

VkShaderModule bcknd_create_shader_module(const char* path, Device* device);
void bcknd_destroy_shader_module(VkShaderModule* mod, Device* device);

void bcknd_create_graphics_shader_pipeline(VkPipeline* pipe, 
        VkPipelineLayout lay, 
        Device* device, 
        Renderpass*  pass, 
        VkShaderModule vertMod, 
        VkShaderModule fragMod, 
        VkExtent2D extent,
        VkFrontFace face);
void bcknd_destroy_shader_pipeline(Device* device, VkPipeline* pipe);