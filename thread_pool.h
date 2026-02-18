#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stdio.h>

#ifdef _OPENMP
    #include <omp.h>
    #define USE_OPENMP 1
#else
    #define USE_OPENMP 0
#endif

typedef struct {
    int start_y;
    int end_y;
    int width;
    int height;
    void *raytracer_data;
    unsigned int *framebuffer;
} RenderTile;

void parallel_render_tile(RenderTile *tile, int num_tiles);

#endif
