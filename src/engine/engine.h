#pragma once

#include "renderer/renderer.h"
#include "window/window.h"

#include "utils.h"

#include <stdbool.h>

typedef struct engine {
    window window;
    renderer renderer;
} engine;

void engine_initialize(engine* engine);
void engine_update(engine* engine);
void engine_render(engine* engine);
void engine_run(engine* engine);
void engine_shutdown(engine* engine);