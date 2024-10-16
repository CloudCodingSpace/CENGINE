#pragma once

#include <vulkan/vulkan.h>

#include "backend/backend.h"
#include "backend/cmd.h"
#include "backend/sync_objs.h"
#include "backend/descriptor.h"

#include "global_data.h"

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

    global_ubo_data uboDat;
    buffer ubo[MAX_FRAMES_IN_FLIGHT];
    void* uboMemMapped[MAX_FRAMES_IN_FLIGHT];

    VkDescriptorPool desc_pool;
    VkDescriptorSet desc_sets[MAX_FRAMES_IN_FLIGHT];
    VkDescriptorSetLayout desc_layout;

    shader shader;
    mesh mesh;

    int crntFrame;
    uint32_t crntImgIdx;
} renderer;

void renderer_initialize(renderer* renderer, window* window);
void renderer_update(renderer* renderer);
void renderer_begin_frame(renderer* renderer, window* window);
void renderer_record_render_cmds(renderer* renderer, uint32_t imgIdx);
void renderer_render(renderer* renderer, window* window);
void renderer_end_frame(renderer* renderer, window* window);
void renderer_shutdown(renderer* renderer);