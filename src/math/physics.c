#include "physics.h"

#include <math.h>

int32_t simulateFall_1Step(double* height, double* velocity) {
  // if it stopped
  if (*height <= 0.01 && fabs(*velocity) <= 1) {
    return -1;
  }

  // speeding up after bounce
  if (*velocity < 0) {
    *velocity += 9;
  } else {
    *velocity += 1;
  }

  *height -= *velocity * 0.05;
  // if bounced
  if (*height <= 0) {
    *height = 0;
    *velocity = -(*velocity) * 0.1;
  }
  return 0;
}