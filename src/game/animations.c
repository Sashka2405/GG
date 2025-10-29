#include "animations.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void drawRGBLine(App* app, int32_t y, uint32_t offset, int32_t isBackward) {
  if (!isBackward) {
    for (int32_t x = 0; x < app->screenWidth; x += app->scalingFactorX) {
      // ??
      int32_t r = (sin((x + offset) * 0.02) + 1) * 127;
      int32_t g = (sin((x + offset) * 0.02 + 2) + 1) * 127;
      int32_t b = (sin((x + offset) * 0.02 + 4) + 1) * 127;

      SDL_SetRenderDrawColor(app->renderer, r, g, b, 255);
      SDL_RenderDrawLine(app->renderer, x, y, x, y + 10);
    }
  } else {
    for (int32_t x = 0; x < app->screenWidth; x += app->scalingFactorX) {
      // ??
      int32_t r = (sin((app->screenWidth - x + offset) * 0.02) + 1) * 127;
      int32_t g = (sin((app->screenWidth - x + offset) * 0.02 + 2) + 1) * 127;
      int32_t b = (sin((app->screenWidth - x + offset) * 0.02 + 4) + 1) * 127;

      SDL_SetRenderDrawColor(app->renderer, r, g, b, 255);
      SDL_RenderDrawLine(app->renderer, x, y, x, y + 10);
    }
  }
}