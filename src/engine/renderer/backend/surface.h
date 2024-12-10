#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <engine/utils.h>
#include "instance.h"

typedef struct {
    VkSurfaceKHR surface;
} Surface;

void create_win_surface(Surface* surface, Instance* inst, GLFWwindow* window);
void destroy_win_surface(Surface* surface, Instance* inst);