#pragma once 

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <engine/utils.h>
#include "debugger.h"

typedef struct instance {
    VkInstance instance;
} instance;

void create_instance(instance* inst);
void destroy_instance(instance* inst);