#include "engine.h"

void engine_initialize(Engine *engine) {
    engine->window.width = 800;
    engine->window.height = 800;
    engine->window.title = "CENGINE";
    window_initialize(&engine->window);

    renderer_initialize(&engine->renderer, &engine->window);
}

void engine_update(Engine *engine) {
    window_update(&engine->window);
    renderer_update(&engine->renderer);
}

void engine_render(Engine *engine) {
    renderer_render(&engine->renderer, &engine->window);
}

void engine_run(Engine *engine) {
    window_show(&engine->window);
    while(true) {
        if(window_is_close_button_pressed(&engine->window)) {
            break;
        }

        engine_update(engine);
        engine_render(engine);
    }

    vkDeviceWaitIdle(engine->renderer.backend.device.logical);
}

void engine_shutdown(Engine *engine) {
    renderer_shutdown(&engine->renderer);
    window_shutdown(&engine->window);
}