#pragma once

#include <cglm/cglm.h>

typedef struct global_ubo_data {
    mat4 proj;
    mat4 view;
    mat4 model;
} global_ubo_data;