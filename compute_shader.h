#ifndef COMPUTE_SHADER_H
#define COMPUTE_SHADER_H

#include <GL/gl.h>

typedef struct {
    GLuint program;
    GLuint input_texture;
    GLuint output_texture;
    unsigned int width;
    unsigned int height;
} ComputeShader;

/*
 * Pseudocode pour future implémentation GPU:
 * 
 * ComputeShader* compute_create(int width, int height);
 * void compute_dispatch(ComputeShader* cs);
 * void compute_destroy(ComputeShader* cs);
 */

/*
 * GLSL Compute Shader Template (raytrace.compute):
 * 
 * #version 430 core
 * layout(local_size_x = 16, local_size_y = 16) in;
 * 
 * layout(rgba32f, binding = 0) uniform image2D outTexture;
 * 
 * struct Sphere {
 *     vec3 position;
 *     float radius;
 *     vec3 color;
 * };
 * 
 * layout(std430, binding = 1) readonly buffer SphereBuffer {
 *     Sphere spheres[];
 * };
 * 
 * void main() {
 *     ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
 *     vec2 uv = pixelCoords / imageSize(outTexture);
 *     
 *     // Ray trace
 *     vec4 color = rayTrace(uv);  // Call to tracing function
 *     imageStore(outTexture, pixelCoords, color);
 * }
 * 
 * Expected Performance:
 * - CPU (current):  ~10 FPS @ 1024×768
 * - GPU (compute):  ~100+ FPS @ 1024×768
 * 
 * Speedup: 10x-100x depending on GPU
 */

#endif
