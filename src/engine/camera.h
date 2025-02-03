#pragma once

#include <cglm/cglm.h>
#include <GLFW/glfw3.h>

typedef struct {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldUp;

    float yaw;
    float pitch;
    float speed;
    float sensitivity;

    mat4 view;
    mat4 projection;
} Camera;

void camera_init(Camera* camera, float fovDegrees, float aspectRatio, float near, float far);
void camera_update(Camera* camera, GLFWwindow* window, float deltaTime);
void camera_update_view(Camera* camera);
void camera_update_projection(Camera* camera, float fovDegrees, float aspectRatio, float near, float far);