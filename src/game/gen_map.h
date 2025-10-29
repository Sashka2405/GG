#ifndef GENMAP_H
#define GENMAP_H

#include <stdint.h>

void genHeightMap(int32_t* heightMap, uint32_t seed, int32_t width,
                  int32_t height);
void addSpawnPlates(int32_t* heightMap, int32_t size, int32_t dx);

#endif