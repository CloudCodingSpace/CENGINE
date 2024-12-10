#include "surface.h"

void create_win_surface(Surface* surface, Instance* inst, GLFWwindow* window) {
    VK_CHECK(glfwCreateWindowSurface(inst->instance, window, 0, &surface->surface))
}

void destroy_win_surface(Surface* surface, Instance* inst) {
    vkDestroySurfaceKHR(inst->instance, surface->surface, 0);
}