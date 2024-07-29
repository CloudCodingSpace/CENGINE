#pragma once

#include <vulkan/vulkan.h>

#include "backend/shader.h"
#include "backend/device.h"
#include "backend/renderpass.h"

typedef struct shader {
     VkShaderModule vertMod;
     VkShaderModule fragMod;
     VkPipeline pipeline;
     VkPipelineLayout pipelineLayout;
} shader;

typedef enum shader_type {
     SHADER_TYPE_INVALID,
     SHADER_TYPE_GRAPHICS,
     SHADER_TYPE_COMPUTE, // TODO
     SHADER_TYPE_RAYTRACING // TODO
} shader_type;

void create_shader(shader* shader,
               shader_type type, 
               device* device,
               renderpass* pass, 
               const char* vertSpvPth, 
               const char* fragSpvPth, 
               VkExtent2D extent);
void bind_shader(shader* shader);
void destroy_shader(shader* shader, device* device);