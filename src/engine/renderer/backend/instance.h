#pragma once 

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <engine/utils.h>
#include "debugger.h"

typedef struct {
    VkInstance instance;
} Instance;

void create_instance(Instance* inst);
void destroy_instance(Instance* inst);