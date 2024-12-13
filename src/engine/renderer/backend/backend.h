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
#include "frameBuffer.h"

typedef struct {
#ifdef _DEBUG
    Debugger debugger;
#endif
    Instance instance;
    Surface surface;
    Device device;
    Swapchain sc;
    Renderpass pass;
    VkFramebuffer* buffs;
} RendererBackend;

void renderer_backend_initialize(RendererBackend* backend, GLFWwindow* window);
void renderer_backend_handle_resize(RendererBackend* backend, GLFWwindow* window);
void renderer_backend_shutdown(RendererBackend* backend);