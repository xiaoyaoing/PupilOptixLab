#pragma once

#include <optix.h>
#include <vector_types.h>

#include "material/optix_material.h"

struct OptixLaunchParams {
    struct {
        unsigned int frame_cnt;
        unsigned int max_depth;

        struct {
            unsigned int width;
            unsigned int height;
        } frame;
    } config;

    float4 *accum_buffer;
    float4 *frame_buffer;

    OptixTraversableHandle handle;
};

struct RayGenData {
};

struct MissData {
};

struct HitGroupData {
    optix_wrap::material::Material mat;
};