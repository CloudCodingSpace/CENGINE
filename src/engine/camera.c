#include "camera.h"

#include "utils.h"

static float lastX, lastY;
static bool firstMouse = true;
static bool capturingMouse = false;

void camera_init(Camera* camera, float fovDegrees, float aspectRatio, float near, float far) {
    glm_vec3_copy((vec3){0.0f, 0.0f, 3.0f}, camera->position);
    glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera->front);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->worldUp);

    camera->yaw = -90.0f;
    camera->pitch = 0.0f;
    camera->speed = 2.5f;
    camera->sensitivity = 0.1f;

    glm_perspective(glm_rad(fovDegrees), aspectRatio, near, far, camera->projection);
    camera_update_view(camera);
}

void camera_update_view(Camera* camera) {
    vec3 center;
    glm_vec3_add(camera->position, camera->front, center);
    glm_lookat(camera->position, center, camera->up, camera->view);
}

void camera_update_projection(Camera* camera, float fovDegrees, float aspectRatio, float near, float far) {
    glm_perspective(glm_rad(fovDegrees), aspectRatio, near, far, camera->projection);
}

void camera_update(Camera* camera, GLFWwindow* window, float deltaTime) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (!capturingMouse) {
            capturingMouse = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    } else {
        if (capturingMouse) {
            capturingMouse = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            firstMouse = true;
        }
    }

    if (capturingMouse) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        
        if (firstMouse) {
            lastX = width / 2.0f;
            lastY = height / 2.0f;
            glfwSetCursorPos(window, lastX, lastY);
            firstMouse = false;
        }

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        xoffset *= camera->sensitivity;
        yoffset *= camera->sensitivity;

        camera->yaw += xoffset;
        camera->pitch += yoffset;

        if (camera->pitch > 89.0f) camera->pitch = 89.0f;
        if (camera->pitch < -89.0f) camera->pitch = -89.0f;
    }

    vec3 front = {
        cosf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch)),
        sinf(glm_rad(camera->pitch)),
        sinf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch))
    };
    
    glm_vec3_normalize_to(front, camera->front);
    glm_vec3_crossn(camera->front, camera->worldUp, camera->right);
    glm_vec3_crossn(camera->right, camera->front, camera->up);
    
    float velocity = camera->speed * deltaTime;
    vec3 temp;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glm_vec3_scale(camera->front, velocity, temp);
        glm_vec3_add(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        glm_vec3_scale(camera->front, velocity, temp);
        glm_vec3_sub(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm_vec3_scale(camera->right, velocity, temp);
        glm_vec3_sub(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm_vec3_scale(camera->right, velocity, temp);
        glm_vec3_add(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        glm_vec3_scale(camera->worldUp, velocity, temp);
        glm_vec3_sub(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        glm_vec3_scale(camera->worldUp, velocity, temp);
        glm_vec3_add(camera->position, temp, camera->position);
    }

    camera_update_view(camera);
}