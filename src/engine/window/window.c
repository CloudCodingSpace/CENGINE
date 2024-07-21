#include "window.h"

void window_initialize(window* window) {
    window->resized = false;

    if(!glfwInit())
        FATAL("Failed to initialize glfw!")
    if(!glfwVulkanSupported())
        FATAL("Vulkan not supported!")

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window->window = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);
    if(!window->window)
        FATAL("Failed to create the window!")
    glfwSetWindowUserPointer(window->window, window);
    glfwSetFramebufferSizeCallback(window->window, window_resize_callback);
}

void window_update(window *window) {
    
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

void window_resize_callback(GLFWwindow *win, int width, int height) {
    window* window = glfwGetWindowUserPointer(win);
    window->width = width;
    window->height = height;
    window->resized = true;
}