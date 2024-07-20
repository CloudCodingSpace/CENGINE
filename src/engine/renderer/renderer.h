#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "backend/backend.h"
#include "backend/cmd.h"
#include "backend/sync_objs.h"
#include <engine/utils.h>

#include <stdint.h>

typedef struct renderer {
    renderer_backend backend;
    VkCommandPool cmdPool;
    VkCommandBuffer cmdBuff;
    VkSemaphore imgAvailableSema;
    VkSemaphore renderFinishedSema;
    VkFence inFlight;
} renderer;

void renderer_initialize(renderer* renderer, GLFWwindow* win_ptr);
void renderer_update(renderer* renderer);
void renderer_record_render_cmds(renderer* renderer, uint32_t imgIdx);
void renderer_render(renderer* renderer);
void renderer_shutdown(renderer* renderer);