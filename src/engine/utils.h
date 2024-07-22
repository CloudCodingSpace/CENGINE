#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#include "logger.h"

#include <stdint.h>
#include <math.h>

void check_vk_result(VkResult result, uint32_t lineNum, const char* funcName, const char* fileName);
uint32_t clamp_uint(uint32_t n, uint32_t min, uint32_t max);
char* read_file(const char* path, size_t* count, const char* mode);

#define MAX_FRAMES_IN_FLIGHT 2

#define VK_CHECK(result) check_vk_result(result, __LINE__, __func__, __FILE__);
#define ARR_SIZE(arr) (sizeof((arr))/sizeof((arr[0])))
#define MAX(a, b) ((a > b) ? (a) : (b)) 
#define MIN(a, b) ((a > b) ? (b) : (a)) 
#define READ_SHADER_FILE(path, size) read_file(path, size, "rb")
#define READ_TEXT_FILE(path, size) read_file(path, size, "rt")
#define CPY_ARR(old, new) {                                                                         \
        for(uint32_t i = 0;i<ARR_SIZE(old); i++) {                                                  \
            new[i] = old[i];                                                                        \
        }                                                                                           \
    }
