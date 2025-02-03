#pragma once

#include "renderer/renderer.h"
#include "window/window.h"
#include "Camera.h"

#include "utils.h"

#include <stdbool.h>

typedef struct {
    Window window;
    Renderer renderer;
    Camera camera;

    float deltaTime;
    float startTime;
} Engine;

void engine_initialize(Engine* engine);
void engine_update(Engine* engine);
void engine_render(Engine* engine);
void engine_run(Engine* engine);
void engine_shutdown(Engine* engine);