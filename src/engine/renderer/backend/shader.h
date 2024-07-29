#pragma once

#include <vulkan/vulkan.h>

#include <engine/utils.h>
#include "device.h"
#include "renderpass.h"

#include "../vertex.h"

VkShaderModule bcknd_create_shader_module(const char* path, device* device);
void bcknd_destroy_shader_module(VkShaderModule* mod, device* device);

void bcknd_create_graphics_shader_pipeline(VkPipeline* pipe, 
            VkPipelineLayout lay, 
            device* device, 
            renderpass* pass,
            VkShaderModule vertMod, 
            VkShaderModule fragMod, 
            VkExtent2D extent);
void bcknd_destroy_shader_pipeline(device* device, VkPipeline* pipe);