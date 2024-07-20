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

typedef struct debugger {
    VkDebugUtilsMessengerEXT debugger;
} debugger;

#define LAYERNAME "VK_LAYER_KHRONOS_validation"

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);


bool check_extensions();
VkDebugUtilsMessengerCreateInfoEXT get_debugger_info();
const char** get_debugger_exts();
uint32_t get_debugger_exts_count();

void create_debugger(debugger* debugger, VkInstance instance);
void destroy_debugger(debugger* debugger, VkInstance instance);

#endif