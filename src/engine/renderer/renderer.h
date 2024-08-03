#pragma once

#include <vulkan/vulkan.h>

#include "backend/backend.h"
#include "backend/cmd.h"
#include "backend/sync_objs.h"

#include "shader.h"
#include "mesh.h"

#include <engine/utils.h>
#include <engine/window/window.h>

#include <stdint.h>

typedef struct renderer {
    renderer_backend backend;

    VkCommandPool cmdPool;
    VkCommandBuffer cmdBuffs[MAX_FRAMES_IN_FLIGHT];
    
    VkSemaphore imgAvailableSemas[MAX_FRAMES_IN_FLIGHT];
    VkSemaphore renderFinishedSemas[MAX_FRAMES_IN_FLIGHT];
    VkFence inFlights[MAX_FRAMES_IN_FLIGHT];

    shader shader;
    mesh mesh;

    int crntFrame;
} renderer;

void renderer_initialize(renderer* renderer, window* window);
void renderer_update(renderer* renderer);
void renderer_record_render_cmds(renderer* renderer, uint32_t imgIdx);
void renderer_render(renderer* renderer, window* window);
void renderer_shutdown(renderer* renderer);