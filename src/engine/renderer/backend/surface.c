#include "surface.h"

void create_win_surface(win_surface *surface, instance* inst, GLFWwindow* window) {
    VK_CHECK(glfwCreateWindowSurface(inst->instance, window, 0, &surface->surface))
}

void destroy_win_surface(win_surface* surface, instance* inst) {
    vkDestroySurfaceKHR(inst->instance, surface->surface, 0);
}