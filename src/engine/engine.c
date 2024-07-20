#include "engine.h"

void engine_initialize(engine *engine) {
    engine->window.width = 800;
    engine->window.height = 800;
    engine->window.title = "CENGINE";
    window_initialize(&engine->window);

    renderer_initialize(&engine->renderer, engine->window.window);
}   

void engine_update(engine *engine) {
    window_update(&engine->window);
    renderer_update(&engine->renderer);
}

void engine_render(engine *engine) {
    renderer_render(&engine->renderer);
}

void engine_run(engine *engine) {
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

void engine_shutdown(engine *engine) {
    renderer_shutdown(&engine->renderer);
    window_shutdown(&engine->window);
}