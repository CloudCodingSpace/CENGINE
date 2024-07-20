#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <engine/utils.h>
#include "instance.h"

typedef struct win_surface {
    VkSurfaceKHR surface;
} win_surface;

void create_win_surface(win_surface* surface, instance* inst, GLFWwindow* window);
void destroy_win_surface(win_surface* surface, instance* inst);