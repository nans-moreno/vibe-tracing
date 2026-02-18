#include "thread_pool.h"

#ifdef _OPENMP
void parallel_render_tile(RenderTile *tiles, int num_tiles) {
    #pragma omp parallel for
    for (int t = 0; t < num_tiles; t++) {
        RenderTile *tile = &tiles[t];
        for (int y = tile->start_y; y < tile->end_y; y++) {
            for (int x = 0; x < tile->width; x++) {
                // Rendering code here
                // tile->framebuffer[y * tile->width + x] = ...
            }
        }
    }
}
#else
void parallel_render_tile(RenderTile *tiles, int num_tiles) {
    // Fallback single-threaded
    for (int t = 0; t < num_tiles; t++) {
        RenderTile *tile = &tiles[t];
        for (int y = tile->start_y; y < tile->end_y; y++) {
            for (int x = 0; x < tile->width; x++) {
                // Rendering code here
                // tile->framebuffer[y * tile->width + x] = ...
            }
        }
    }
}
#endif
