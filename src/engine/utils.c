#include "utils.h"

void check_vk_result(VkResult result, uint32_t lineNum, const char* funcName, const char* fileName) {
    if(result != VK_SUCCESS) {
        char* msg = "VkResult is %s (line: %d, function: %s, file: %s)";
        FATAL(msg, string_VkResult(result), lineNum, funcName, fileName);
    }
}

uint32_t clamp_uint(uint32_t n, uint32_t min, uint32_t max) {
    if(n < min) return min;
    if(n > max) return max;
    return n;
}

char* read_file(const char* path, size_t* count, const char* mode) {
    FILE* file = fopen(path, mode);
    fseek(file, 0L, SEEK_END);
    *count = ftell(file);
    fseek(file, 0L, SEEK_SET);
    char* content = (char*) calloc((*count) + 1, sizeof(char));
    size_t read = fread(content, *count, sizeof(char), file);
    content[*count] = '\0';

    return content;
}