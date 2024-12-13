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

typedef struct {
    RendererBackend backend;

    VkCommandPool cmdPool;
    VkCommandBuffer cmdBuffs[MAX_FRAMES_IN_FLIGHT];
    
    VkSemaphore imgAvailableSemas[MAX_FRAMES_IN_FLIGHT];
    VkSemaphore renderFinishedSemas[MAX_FRAMES_IN_FLIGHT];
    VkFence inFlights[MAX_FRAMES_IN_FLIGHT];

    global_ubo_data uboDat;
    Buffer ubo[MAX_FRAMES_IN_FLIGHT];
    void* uboMemMapped[MAX_FRAMES_IN_FLIGHT];

    VkDescriptorPool desc_pool;
    VkDescriptorSet desc_sets[MAX_FRAMES_IN_FLIGHT];
    VkDescriptorSetLayout desc_layout;

    Shader shader;
    Mesh Mesh;

    int crntFrame;
    uint32_t crntImgIdx;
} Renderer;

void renderer_initialize(Renderer* renderer, Window* window);
void renderer_update(Renderer* renderer);
void renderer_begin_frame(Renderer* renderer, Window* window);
void renderer_record_render_cmds(Renderer* renderer, uint32_t imgIdx);
void renderer_render(Renderer* renderer, Window* window);
void renderer_end_frame(Renderer* renderer, Window* window);
void renderer_shutdown(Renderer* renderer);