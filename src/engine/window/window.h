#pragma once

#include <GLFW/glfw3.h>

#include <engine/logger.h> 
#include <engine/utils.h> 
#include <stdbool.h>

typedef struct window {
    GLFWwindow* window;
    int width, height;
    const char* title;
    bool resized;
} window;

void window_initialize(window* window);
void window_update(window* window);
void window_show(window* window);
void window_shutdown(window* window);

bool window_is_close_button_pressed(window* window);