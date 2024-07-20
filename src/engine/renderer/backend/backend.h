#pragma once

#include <engine/logger.h>
#include <engine/utils.h>

#include <stdlib.h>

#include "instance.h"
#include "swapchain.h"
#include "surface.h"
#include "device.h"
#include "debugger.h"
#include "renderpass.h"

typedef struct renderer_backend {
#ifdef _DEBUG
    debugger debugger;
#endif
    instance instance;
    win_surface surface;
    device device;
    swapchain sc;
    renderpass pass;
    VkFramebuffer* buffs;
} renderer_backend;

void renderer_backend_initialize(renderer_backend* backend, GLFWwindow* window);
void renderer_backend_shutdown(renderer_backend* backend);