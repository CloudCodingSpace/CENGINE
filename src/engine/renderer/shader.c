#include "shader.h"

void create_shader(shader* shader,
               shader_type type, 
               Device* device,
               Renderpass*  pass, 
               const char* vertSpvPth, 
               const char* fragSpvPth, 
               VkExtent2D extent,
               VkFrontFace face, 
               uint32_t desc_layout_count, 
               VkDescriptorSetLayout* desc_layouts) {
    shader->vertMod = bcknd_create_shader_module(vertSpvPth, device);
    shader->fragMod = bcknd_create_shader_module(fragSpvPth, device);
    shader->type = type;

    // Pipeline Layout 
    {
        VkPipelineLayoutCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO
        };

        if(desc_layout_count > 0) {
            info.setLayoutCount = desc_layout_count,
            info.pSetLayouts = desc_layouts;
        }

        VK_CHECK(vkCreatePipelineLayout(device->logical, &info, 0, &shader->pipelineLayout))
    }

    if(type == SHADER_TYPE_INVALID) {
        FATAL("Shader type is invalid!\n");
    }
    else if(type == SHADER_TYPE_GRAPHICS) {
        bcknd_create_graphics_shader_pipeline(&shader->pipeline, 
                shader->pipelineLayout, 
                device, 
                pass, 
                shader->vertMod, 
                shader->fragMod, 
                extent, 
                face);
    } else if (type == SHADER_TYPE_COMPUTE) {
        // TODO
    } else {
        // TODO
    }
}

void bind_shader(shader* shader, VkCommandBuffer* buff, bool bindDesc, VkDescriptorSet* set) {
    VkPipelineBindPoint point;
    if(shader->type == SHADER_TYPE_GRAPHICS)
        point = VK_PIPELINE_BIND_POINT_GRAPHICS;
    if(shader->type == SHADER_TYPE_COMPUTE)
        point = VK_PIPELINE_BIND_POINT_COMPUTE;

    if(bindDesc) {
        vkCmdBindDescriptorSets(*buff, point, shader->pipelineLayout, 0, 1, set, 0, 0);
    }

    vkCmdBindPipeline(*buff, point, shader->pipeline);
}

void destroy_shader(shader* shader, Device* device) {
    bcknd_destroy_shader_module(&shader->vertMod, device);
    bcknd_destroy_shader_module(&shader->fragMod, device);
    vkDestroyPipelineLayout(device->logical, shader->pipelineLayout, 0);
    bcknd_destroy_shader_pipeline(device, &shader->pipeline);
}