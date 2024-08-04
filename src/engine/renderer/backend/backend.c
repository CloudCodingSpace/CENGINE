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
    for(uint32_t i = 0; i < ctx->sc.imgCount; i++) {
        VkImageView view[] = {
            ctx->sc.imgViews[i]
        };

        create_framebuffer(&ctx->buffs[i], &ctx->device, view, ARR_SIZE(view), ctx->sc.extent, &ctx->pass);
    }
}

void renderer_backend_handle_resize(renderer_backend *ctx, GLFWwindow *window) {
    // Handling minimize
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while(width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(ctx->device.logical);
    // Destroying objs
    destroy_swapchain(&ctx->sc, &ctx->device);
    for(uint32_t i = 0; i < ctx->sc.imgCount; i++) {
        destroy_framebuffer(&ctx->buffs[i], &ctx->device);
    }
    // Recreating objs
    create_swapchain(&ctx->sc, &ctx->instance, &ctx->device, &ctx->surface, window);
    for(uint32_t i = 0; i < ctx->sc.imgCount; i++) {
        VkImageView view[] = {
            ctx->sc.imgViews[i]
        };

        create_framebuffer(&ctx->buffs[i], &ctx->device, view, ARR_SIZE(view), ctx->sc.extent, &ctx->pass);
    }
}

void renderer_backend_shutdown(renderer_backend* ctx) {
    for(uint32_t i = 0; i < ctx->sc.imgCount; i++) {
        destroy_framebuffer(&ctx->buffs[i], &ctx->device);
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