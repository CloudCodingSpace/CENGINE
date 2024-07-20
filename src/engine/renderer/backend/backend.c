#include "backend.h"

void renderer_backend_initialize(renderer_backend* ctx, GLFWwindow* window) {
#ifdef _DEBUG
    if(!check_extensions()) {
        FATAL("Debug mode but debugger extensions not available!")
    }
#endif
    create_instance(&ctx->instance);
#ifdef _DEBUG
    create_debugger(&ctx->debugger, ctx->instance.instance);
#endif
    create_win_surface(&ctx->surface, &ctx->instance, window);
    create_device(&ctx->device, &ctx->instance, ctx->surface.surface);
    create_swapchain(&ctx->sc, &ctx->instance, &ctx->device, &ctx->surface, window);
    create_renderpass(&ctx->pass, &ctx->device, &ctx->sc);

    ctx->buffs = (VkFramebuffer*) calloc(ctx->sc.imgCount, sizeof(VkFramebuffer));
    // VkFramebuffer buffs[framebuffCount];
    for(uint32_t i = 0; i < ctx->sc.imgCount; i++) {
        VkImageView view[] = {
            ctx->sc.imgViews[i]
        };

        VkFramebufferCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .attachmentCount = 1,
            .pAttachments = view,
            .layers = 1,
            .width = ctx->sc.extent.width,
            .height = ctx->sc.extent.height,
            .renderPass = ctx->pass.pass
        };

        VK_CHECK(vkCreateFramebuffer(ctx->device.logical, &info, 0, &ctx->buffs[i]))
    }
}

void renderer_backend_shutdown(renderer_backend* ctx) {
    for(uint32_t i = 0; i < ctx->sc.imgCount; i++) {
        vkDestroyFramebuffer(ctx->device.logical, ctx->buffs[i], 0);
    }
    free(ctx->buffs);

    destroy_renderpass(&ctx->pass, &ctx->device);
    destroy_swapchain(&ctx->sc, &ctx->device);
    destroy_device(&ctx->device);
    destroy_win_surface(&ctx->surface, &ctx->instance);
#ifdef _DEBUG
    destroy_debugger(&ctx->debugger, ctx->instance.instance);
#endif
    destroy_instance(&ctx->instance);
}