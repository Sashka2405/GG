#include "gen_map.h"

#include "../math/perlin.h"

void addSpawnPlates(int32_t* heightMap, int32_t size, int32_t dx) {
  int32_t leftX = heightMap[dx - 1];
  int32_t rightX = heightMap[size - 2 - dx];
  for (int32_t i = 0; i < dx; ++i) {
    heightMap[i] = leftX;
    heightMap[size - i - 1] = rightX;
  }
}

void genHeightMap(int32_t* heightMap, uint32_t seed, int32_t width,
                  int32_t height) {
  generateHeightmap(heightMap, seed, width, height);
}