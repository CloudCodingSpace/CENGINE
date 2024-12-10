#pragma once

#include <GLFW/glfw3.h>

#include <engine/logger.h> 
#include <engine/utils.h> 
#include <stdbool.h>

typedef struct {
    GLFWwindow* window;
    int width, height;
    const char* title;
    bool resized;
} Window;

void window_initialize(Window* window);
void window_update(Window* window);
void window_show(Window* window);
void window_shutdown(Window* window);

bool window_is_close_button_pressed(Window* window);