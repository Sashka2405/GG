#ifndef RAND_H
#define RAND_H

#include <stdint.h>

int32_t getRandomValue(int32_t min, int32_t max);

#if defined(__unix__)
int32_t getRandomDWORD_unix();
#elif defined(_WIN32)
int32_t getRandomDWORD_win();
#endif

#endif