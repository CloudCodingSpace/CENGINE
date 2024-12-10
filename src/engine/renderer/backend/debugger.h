#pragma once

#ifdef _DEBUG

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <engine/logger.h>
#include <engine/utils.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    VkDebugUtilsMessengerEXT debugger;
} Debugger;

#define DBG_LAYERNAME "VK_LAYER_KHRONOS_validation"

bool check_dbg_extensions();
VkDebugUtilsMessengerCreateInfoEXT get_debugger_info();
const char** get_debugger_exts();
uint32_t get_debugger_exts_count();

void create_debugger(Debugger* debugger, VkInstance instance);
void destroy_debugger(Debugger* debugger, VkInstance instance);

#endif