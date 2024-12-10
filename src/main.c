#include <stdio.h>
#include <conio.h>

#include "engine/engine.h"

int main(int argc, const char** argv) {
    Engine engine;
    engine_initialize(&engine);
    engine_run(&engine);
    engine_shutdown(&engine);

    printf("Press any key to continue...\n");
    getch();
    return 0;
}