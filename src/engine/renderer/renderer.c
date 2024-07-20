#include "renderer.h"

void renderer_initialize(renderer *renderer, GLFWwindow* win_ptr) {
    renderer_backend_initialize(&renderer->backend, win_ptr);
    create_cmd_pool(&renderer->cmdPool, &renderer->backend.device);
    renderer->cmdBuff = create_command_buff(renderer->cmdPool, &renderer->backend.device);
    create_semaphore(&renderer->imgAvailableSema, &renderer->backend.device);
    create_semaphore(&renderer->renderFinishedSema, &renderer->backend.device);
    create_fence(&renderer->inFlight, &renderer->backend.device);
}

void renderer_update(renderer *renderer) {

}

void renderer_record_render_cmds(renderer *renderer, uint32_t imgIdx) {
    begin_cmd_buff(&renderer->backend.device, &renderer->cmdBuff);

    VkClearValue clear = {{0.1f, 0.1f, 0.1f, 1.0f}};
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
    vkCmdBeginRenderPass(renderer->cmdBuff, &passBegin, VK_SUBPASS_CONTENTS_INLINE);
    

    
    vkCmdEndRenderPass(renderer->cmdBuff);
    end_cmd_buff(&renderer->cmdBuff);
}

void renderer_render(renderer *renderer) {
    vkWaitForFences(renderer->backend.device.logical, 1, &renderer->inFlight, VK_TRUE, UINT64_MAX);
    vkResetFences(renderer->backend.device.logical, 1, &renderer->inFlight);

    uint32_t imgIdx;
    VK_CHECK(vkAcquireNextImageKHR(renderer->backend.device.logical, renderer->backend.sc.swapchain, UINT64_MAX, renderer->imgAvailableSema, VK_NULL_HANDLE, &imgIdx))

    VK_CHECK(vkResetCommandBuffer(renderer->cmdBuff, 0))
    renderer_record_render_cmds(renderer, imgIdx);

    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &renderer->imgAvailableSema,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &renderer->renderFinishedSema,
        .commandBufferCount = 1,
        .pCommandBuffers = &renderer->cmdBuff,
        .pWaitDstStageMask = waitStages
    };

    VK_CHECK(vkQueueSubmit(renderer->backend.device.families.queue, 1, &submitInfo, renderer->inFlight))

    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pImageIndices = &imgIdx,
        .swapchainCount = 1,
        .pSwapchains = &renderer->backend.sc.swapchain,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &renderer->renderFinishedSema
    };

    VK_CHECK(vkQueuePresentKHR(renderer->backend.device.families.queue, &presentInfo))
}

void renderer_shutdown(renderer *renderer) {
    destroy_semaphore(&renderer->imgAvailableSema, &renderer->backend.device);
    destroy_semaphore(&renderer->renderFinishedSema, &renderer->backend.device);
    destroy_fence(&renderer->inFlight, &renderer->backend.device);
    destroy_cmd_pool(&renderer->cmdPool, &renderer->backend.device);
    renderer_backend_shutdown(&renderer->backend);
}