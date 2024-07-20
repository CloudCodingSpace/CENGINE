#include "window.h"

void window_initialize(window* window) {
    if(!glfwInit())
        FATAL("Failed to initialize glfw!")
    if(!glfwVulkanSupported())
        FATAL("Vulkan not supported!")

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window->window = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);
    if(!window->window)
        FATAL("Failed to create the window!")
}

void window_update(window *window) {
    glfwGetFramebufferSize(window->window, &window->width, &window->height);
    glfwPollEvents();
}

void window_show(window *window) {
    glfwShowWindow(window->window);
}

void window_shutdown(window* window) {
    glfwDestroyWindow(window->window);
    glfwTerminate();
}

bool window_is_close_button_pressed(window *window) {
    return glfwWindowShouldClose(window->window);
}