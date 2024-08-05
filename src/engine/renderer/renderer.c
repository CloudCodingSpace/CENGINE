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

        create_shader(&renderer->shader, SHADER_TYPE_GRAPHICS, &renderer->backend.device, &renderer->backend.pass, "shaders/default.vert.spv", "shaders/default.frag.spv", renderer->backend.sc.extent);
        create_mesh(&renderer->mesh, &renderer->backend.device, &renderer->cmdPool, vertices, indices, ARR_SIZE(vertices), ARR_SIZE(indices));
    }
}

void renderer_update(renderer* renderer) {

}

void renderer_record_render_cmds(renderer* renderer, uint32_t imgIdx) {
    bcknd_begin_cmd_buff(&renderer->backend.device, &renderer->cmdBuffs[renderer->crntFrame]);

    VkClearValue clear = {{{0.01f, 0.01f, 0.01f, 1.0f}}};
    VkFramebuffer buff = renderer->backend.buffs[imgIdx];
    VkRenderPassBeginInfo passBegin = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .clearValueCount = 1,
        .pClearValues = &clear,
        .framebuffer = buff,
        .renderArea.offset = {0, 0},
        .renderArea.extent = renderer->backend.sc.extent,
        .renderPass = renderer->backend.pass.pass
    };
    vkCmdBeginRenderPass(renderer->cmdBuffs[renderer->crntFrame], &passBegin, VK_SUBPASS_CONTENTS_INLINE);

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

    bind_shader(&renderer->shader, &renderer->cmdBuffs[renderer->crntFrame]);
    render_mesh(&renderer->mesh, &renderer->cmdBuffs[renderer->crntFrame]);

    vkCmdEndRenderPass(renderer->cmdBuffs[renderer->crntFrame]);
    bcknd_end_cmd_buff(&renderer->cmdBuffs[renderer->crntFrame]);
}

void renderer_render(renderer* renderer, window* window) {
    VK_CHECK(vkWaitForFences(renderer->backend.device.logical, 1, &renderer->inFlights[renderer->crntFrame], VK_TRUE, UINT64_MAX))
    VK_CHECK(vkResetFences(renderer->backend.device.logical, 1, &renderer->inFlights[renderer->crntFrame]))

    uint32_t imgIdx;
    VkResult result = vkAcquireNextImageKHR(renderer->backend.device.logical, renderer->backend.sc.swapchain, UINT64_MAX, renderer->imgAvailableSemas[renderer->crntFrame], VK_NULL_HANDLE, &imgIdx);
    if(result == VK_ERROR_OUT_OF_DATE_KHR) {
        renderer_backend_handle_resize(&renderer->backend, window->window);
        return;
    }
    if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        char* msg = "VkResult is %s (line: %d, function: %s, file: %s)";
        FATAL(msg, string_VkResult(result), __LINE__, __func__, __FILE__);
    }

    VK_CHECK(vkResetFences(renderer->backend.device.logical, 1, &renderer->inFlights[renderer->crntFrame]))

    VK_CHECK(vkResetCommandBuffer(renderer->cmdBuffs[renderer->crntFrame], 0))
    renderer_record_render_cmds(renderer, imgIdx);

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
        .pImageIndices = &imgIdx,
        .swapchainCount = 1,
        .pSwapchains = &renderer->backend.sc.swapchain,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &renderer->renderFinishedSemas[renderer->crntFrame]
    };

    result = vkQueuePresentKHR(renderer->backend.device.families.queue, &presentInfo);
    if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window->resized) {
        renderer_backend_handle_resize(&renderer->backend, window->window);
    } else if(result != VK_SUCCESS) {
        char* msg = "VkResult is %s (line: %d, function: %s, file: %s)";
        FATAL(msg, string_VkResult(result), __LINE__, __func__, __FILE__);
    }

    renderer->crntFrame = (renderer->crntFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void renderer_shutdown(renderer* renderer) {
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
