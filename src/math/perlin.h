#ifndef PERLIN_H
#define PERLIN_H

#include <stdint.h>

float perlin1d(float x);
float perlin1d_octaves(float x, int32_t octaves, float persistence);
void generatePermutation(uint32_t seed);
void generateHeightmap(int32_t* heightmap, uint32_t seed, int32_t WIDTH,
                       int32_t HEIGHT);
void showHeightMapUTF8(int32_t* heightmap, int32_t WIDTH, int32_t HEIGHT);

#endif