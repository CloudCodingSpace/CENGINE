#pragma once

#include <vulkan/vulkan.h>

#include "backend/shader.h"
#include "backend/device.h"
#include "backend/renderpass.h"

typedef enum {
     SHADER_TYPE_INVALID,
     SHADER_TYPE_GRAPHICS,
     SHADER_TYPE_COMPUTE, // TODO
     SHADER_TYPE_RAYTRACING // TODO
} ShaderType;

typedef struct {
     VkShaderModule vertMod;
     VkShaderModule fragMod;
     VkPipeline pipeline;
     VkPipelineLayout pipelineLayout;

     ShaderType type;
} Shader;

void create_shader(Shader* shader,
               ShaderType type, 
               Device* device,
               Renderpass*  pass, 
               const char* vertSpvPth, 
               const char* fragSpvPth, 
               VkExtent2D extent,
               VkFrontFace face, 
               uint32_t desc_layout_count, 
               VkDescriptorSetLayout* desc_layouts);
void bind_shader(Shader* shader, VkCommandBuffer* buff, bool bindDesc, VkDescriptorSet* set);
void destroy_shader(Shader* shader, Device* device);