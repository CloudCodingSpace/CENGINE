#include "renderer.h"

void renderer_initialize(renderer* renderer, window* window) {
    // Renderering setup
    {
        renderer->crntFrame = 0;

        renderer_backend_initialize(&renderer->backend, window->window);
        bcknd_create_cmd_pool(&renderer->cmdPool, &renderer->backend.device);
        for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            renderer->cmdBuffs[i] = bcknd_create_command_buff(renderer->cmdPool, &renderer->backend.device);
            bcknd_create_semaphore(&renderer->imgAvailableSemas[i], &renderer->backend.device);
            bcknd_create_semaphore(&renderer->renderFinishedSemas[i], &renderer->backend.device);
            bcknd_create_fence(&renderer->inFlights[i], &renderer->backend.device);
        }
    }
    // UBO stuff
    {
        // Creating the uniform buffer
        for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            create_ubo(&renderer->ubo[i], &renderer->backend.device, sizeof(global_ubo_data));
            VK_CHECK(vkMapMemory(renderer->backend.device.logical, renderer->ubo[i].mem, 0, sizeof(global_ubo_data), 0, &(renderer->uboMemMapped[i])))
        }

        // Creating the descriptors
        VkDescriptorPoolSize poolSizes[1];
        poolSizes[0] = (VkDescriptorPoolSize) {
            .descriptorCount = MAX_FRAMES_IN_FLIGHT,
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
        };

        bcknd_create_desc_pool(&renderer->desc_pool, &renderer->backend.device, MAX_FRAMES_IN_FLIGHT, ARR_SIZE(poolSizes), poolSizes);

        VkDescriptorSetLayoutBinding layBindings[1];
        layBindings[0] = (VkDescriptorSetLayoutBinding){
            .binding = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT
        };

        bcknd_create_desc_layout(&renderer->desc_layout, &renderer->backend.device, ARR_SIZE(layBindings), layBindings);

        VkDescriptorSetLayout lays[MAX_FRAMES_IN_FLIGHT];
        for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            lays[i] = renderer->desc_layout;
        }

        bcknd_create_desc_set(renderer->desc_sets, MAX_FRAMES_IN_FLIGHT, &renderer->backend.device, renderer->desc_pool, lays);

        for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VkDescriptorBufferInfo info = {
                .buffer = renderer->ubo[i].buff,
                .offset = 0,
                .range = sizeof(global_ubo_data)
            };

            VkWriteDescriptorSet write = {
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount = 1,
                .dstSet = renderer->desc_sets[i],
                .dstBinding = 0,
                .dstArrayElement = 0,
                .pBufferInfo = &info
            };

            vkUpdateDescriptorSets(renderer->backend.device.logical, 1, &write, 0, 0);
        }
        
    }
    // Rendering objects
    {
        vertex vertices[] = {
            // Pos                  // Colors
            {{ 0.0f, -0.5f, 0.0f},  {1.0f, 0.0f, 0.0f}},
            {{ 0.5f,  0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}},
            {{-0.5f,  0.5f, 0.0f},  {0.0f, 0.0f, 1.0f}}
        };

        uint32_t indices[] = {
            0, 1, 2
        };

        create_shader(&renderer->shader, SHADER_TYPE_GRAPHICS, &renderer->backend.device, &renderer->backend.pass, "shaders/default.vert.spv", "shaders/default.frag.spv", renderer->backend.sc.extent, VK_FRONT_FACE_COUNTER_CLOCKWISE, 1, &renderer->desc_layout);
        create_mesh(&renderer->mesh, &renderer->backend.device, &renderer->cmdPool, vertices, indices, ARR_SIZE(vertices), ARR_SIZE(indices));
    }
}

void renderer_update(renderer* renderer) {
    
}

void renderer_begin_frame(renderer* renderer, window* window) {
    VK_CHECK(vkWaitForFences(renderer->backend.device.logical, 1, &renderer->inFlights[renderer->crntFrame], VK_TRUE, UINT64_MAX))
    VK_CHECK(vkResetFences(renderer->backend.device.logical, 1, &renderer->inFlights[renderer->crntFrame]))

    VkResult result = vkAcquireNextImageKHR(renderer->backend.device.logical, renderer->backend.sc.swapchain, UINT64_MAX, renderer->imgAvailableSemas[renderer->crntFrame], VK_NULL_HANDLE, &renderer->crntImgIdx);
    if(result == VK_ERROR_OUT_OF_DATE_KHR) {
        renderer_backend_handle_resize(&renderer->backend, window->window);
        return;
    }
    if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        char* msg = "VkResult is %s (line: %d, function: %s, file: %s)";
        FATAL(msg, string_VkResult(result), __LINE__, __func__, __FILE__);
    }

    // Updating ubo data
    {
        global_ubo_data data;
        glm_mat4_identity(data.model);
        glm_mat4_identity(data.view);
        glm_mat4_identity(data.proj);

        glm_rotate(data.model, glfwGetTime() * glm_rad(90.0f), (vec3){0.0f, 0.0f, 1.0f});
        glm_lookat((vec3){2.0f, 2.0f, 2.0f}, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 0.0f, 1.0f}, data.view);
        glm_perspective(glm_rad(90.0f), renderer->backend.sc.extent.width / (float) renderer->backend.sc.extent.height, 0.1f, 100.0f, data.proj);

        data.proj[1][1] *= -1.0f;

        memcpy(renderer->uboMemMapped[renderer->crntFrame], &data, sizeof(data));
    }

    VK_CHECK(vkResetFences(renderer->backend.device.logical, 1, &renderer->inFlights[renderer->crntFrame]))

    VK_CHECK(vkResetCommandBuffer(renderer->cmdBuffs[renderer->crntFrame], 0))
}

void renderer_record_render_cmds(renderer* renderer, uint32_t imgIdx) {
    // Code for initializing Vulkan for rendering or in other words actually recording render commands to a command buffer
    {
        bcknd_begin_cmd_buff(&renderer->backend.device, &renderer->cmdBuffs[renderer->crntFrame]);

        VkClearValue clear = {{{0.01f, 0.01f, 0.01f, 1.0f}}};
        VkFramebuffer buff = renderer->backend.buffs[imgIdx];
        
        begin_renderpass(&renderer->backend.pass, renderer->cmdBuffs[renderer->crntFrame], buff, clear, renderer->backend.sc.extent, (VkOffset2D){0, 0});

        // Setting the pipeline viewport and scissor
        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)renderer->backend.sc.extent.width;
        viewport.height = (float)renderer->backend.sc.extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(renderer->cmdBuffs[renderer->crntFrame], 0, 1, &viewport);

        VkRect2D scissor = {};
        scissor.offset = (VkOffset2D){0, 0};
        scissor.extent = renderer->backend.sc.extent;
        vkCmdSetScissor(renderer->cmdBuffs[renderer->crntFrame], 0, 1, &scissor);
    }

    // Actual rendering commands for rendering and binding objects
    {
        bind_shader(&renderer->shader, &renderer->cmdBuffs[renderer->crntFrame], true, &renderer->desc_sets[renderer->crntFrame]);
        render_mesh(&renderer->mesh, &renderer->cmdBuffs[renderer->crntFrame]);
    }

    end_renderpass(renderer->cmdBuffs[renderer->crntFrame]);
    bcknd_end_cmd_buff(&renderer->cmdBuffs[renderer->crntFrame]);
}

void renderer_render(renderer* renderer, window* window) {
    renderer_begin_frame(renderer, window);
    renderer_record_render_cmds(renderer, renderer->crntImgIdx);
    renderer_end_frame(renderer, window);

    renderer->crntFrame = (renderer->crntFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void renderer_end_frame(renderer *renderer, window* window) {
    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &renderer->imgAvailableSemas[renderer->crntFrame],
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &renderer->renderFinishedSemas[renderer->crntFrame],
        .commandBufferCount = 1,
        .pCommandBuffers = &renderer->cmdBuffs[renderer->crntFrame],
        .pWaitDstStageMask = waitStages
    };

    VK_CHECK(vkQueueSubmit(renderer->backend.device.families.queue, 1, &submitInfo, renderer->inFlights[renderer->crntFrame]))

    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pImageIndices = &renderer->crntImgIdx,
        .swapchainCount = 1,
        .pSwapchains = &renderer->backend.sc.swapchain,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &renderer->renderFinishedSemas[renderer->crntFrame]
    };

    VkResult result = vkQueuePresentKHR(renderer->backend.device.families.queue, &presentInfo);
    if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window->resized) {
        renderer_backend_handle_resize(&renderer->backend, window->window);
    } else if(result != VK_SUCCESS) {
        char* msg = "VkResult is %s (line: %d, function: %s, file: %s)";
        FATAL(msg, string_VkResult(result), __LINE__, __func__, __FILE__);
    }
}

void renderer_shutdown(renderer* renderer) {
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        destroy_buffer(&renderer->ubo[i], &renderer->backend.device);
    }

    bcknd_destroy_desc_layout(&renderer->desc_layout, &renderer->backend.device);
    bcknd_destroy_desc_pool(&renderer->desc_pool, &renderer->backend.device);

    destroy_mesh(&renderer->mesh, &renderer->backend.device);
    destroy_shader(&renderer->shader, &renderer->backend.device);

    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        bcknd_destroy_semaphore(&renderer->imgAvailableSemas[i], &renderer->backend.device);
        bcknd_destroy_semaphore(&renderer->renderFinishedSemas[i], &renderer->backend.device);
        bcknd_destroy_fence(&renderer->inFlights[i], &renderer->backend.device);
    }
    bcknd_destroy_cmd_pool(&renderer->cmdPool, &renderer->backend.device);
    renderer_backend_shutdown(&renderer->backend);
}
