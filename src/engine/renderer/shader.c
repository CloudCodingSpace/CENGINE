#include "shader.h"

void create_shader(shader* shader,
        shader_type type,
        device* device,
        renderpass* pass,
        const char* vertSpvPth, 
        const char* fragSpvPth, 
        VkExtent2D extent) {
    shader->vertMod = bcknd_create_shader_module(vertSpvPth, device);
    shader->fragMod = bcknd_create_shader_module(fragSpvPth, device);

    // Pipeline Layout 
    {
        VkPipelineLayoutCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO
        };

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
                extent);
    } else if (type == SHADER_TYPE_COMPUTE) {
        // TODO
    } else {
        // TODO
    }
}

void bind_shader(shader* shader) {
    
}

void destroy_shader(shader* shader, device* device) {
    bcknd_destroy_shader_module(&shader->vertMod, device);
    bcknd_destroy_shader_module(&shader->fragMod, device);
    vkDestroyPipelineLayout(device->logical, shader->pipelineLayout, 0);
    bcknd_destroy_shader_pipeline(device, &shader->pipeline);
}