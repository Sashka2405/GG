#include "perlin.h"

#include <math.h>
#include <stdlib.h>

#define PERLIN_SIZE 256

int32_t perm[PERLIN_SIZE * 2];

float fade(float t) {
  return t * t * t * (t * (t * 6 - 15) + 10);
}
float lerp(float a, float b, float t) {
  return a + t * (b - a);
}
int32_t grad(int32_t hash) {
  return (hash & 1) ? 1 : -1;
}

float perlin1d(float x) {
  int32_t X = (int32_t)floor(x) & 255;
  float xf = x - floor(x);
  float u = fade(xf);
  int32_t g1 = grad(perm[X]);
  int32_t g2 = grad(perm[X + 1]);
  float dot1 = g1 * xf;
  float dot2 = g2 * (xf - 1);
  return lerp(dot1, dot2, u);
}

float perlin1d_octaves(float x, int32_t octaves, float persistence) {
  float total = 0, amplitude = 1, maxValue = 0;
  for (int32_t i = 0; i < octaves; i++) {
    total += perlin1d(x * pow(2, i)) * amplitude;
    maxValue += amplitude;
    amplitude *= persistence;
  }
  return total / maxValue;
}

void generatePermutation(uint32_t seed) {
  int32_t basePerm[PERLIN_SIZE];
  for (int32_t i = 0; i < PERLIN_SIZE; i++) basePerm[i] = i;
  srand(seed);
  for (int32_t i = 0; i < PERLIN_SIZE; i++) {
    int32_t j = rand() % PERLIN_SIZE;
    int32_t temp = basePerm[i];
    basePerm[i] = basePerm[j];
    basePerm[j] = temp;
  }
  for (int32_t i = 0; i < PERLIN_SIZE; i++) {
    perm[i] = basePerm[i];
    perm[i + PERLIN_SIZE] = basePerm[i];
  }
}

void generateHeightmap(int32_t* heightmap, uint32_t seed, int32_t WIDTH,
                       int32_t HEIGHT) {
  generatePermutation(seed);

  float baseScale = 0.005;
  float rareScale = 0.01;
  int32_t baseOctaves = 4;
  int32_t rareOctaves = 2;
  float persistence1 = 0.5;
  float persistence2 = 1.f;

  float scaleFactor = 512.0f / WIDTH;
  float scale1 = baseScale * scaleFactor;
  float scale2 = rareScale * scaleFactor;

  int32_t octaves1 = baseOctaves + (WIDTH > 512 ? 2 : 0);
  int32_t octaves2 = rareOctaves + (WIDTH > 512 ? 1 : 0);
  for (int32_t x = 0; x < WIDTH; x++) {
    float baseNoise = perlin1d_octaves(x * scale1, octaves1, persistence1);
    float rareNoise = perlin1d_octaves(x * scale2, octaves2, persistence2);
    rareNoise = pow(fabs(rareNoise), 4) * (rareNoise > 0 ? 1 : -1);

    float heightValue = baseNoise + rareNoise;
    heightmap[x] = HEIGHT - (int32_t)((heightValue + 1) * 0.7 * HEIGHT * 0.7);
  }
}