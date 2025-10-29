#include "wind.h"

//#include <>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <stdint.h>

#include "../../SDL/SDL_render.h"
#include "../../SDL/ui_helpers.h"
#include "../../math/math.h"
#include "../../math/rand.h"
#include "log/log.h"
#include "windStruct.h"

void updateWind(App* app) {
  Wind* wind = &app->globalConditions.wind;

  wind->windStrength = getRandomValue(10, 100) / 10.;
  wind->windDirection = getRandomValue(0, 6);

  updateWindDirectionIcon(wind->directionIcon, wind->windDirection);
  updateWindSpeedLabel(app, wind->speedLabel, wind->windStrength);

  wind->windStrength *= 2;
}

void updateWindDirectionIcon(RenderObject* directionIcon,
                             enum WindDirection direction) {
  switch (direction) {
    case E:
      directionIcon->data.texture.angle = 0;
      break;
    case SE:
      directionIcon->data.texture.angle = 45;
      break;
    case SW:
      directionIcon->data.texture.angle = 135;
      break;
    case W:
      directionIcon->data.texture.angle = 180;
      break;
    case NW:
      directionIcon->data.texture.angle = 225;
      break;
    case NE:
      directionIcon->data.texture.angle = 315;
      break;
  }
}

void updateWindSpeedLabel(App* app, RenderObject* speedLabel,
                          double windStrength) {
  int32_t minWindSpeedLabel, maxWindSpeedLabel;
  minWindSpeedLabel = floor(windStrength) - getRandomValue(0, 1);
  minWindSpeedLabel = MAX(minWindSpeedLabel, 0);

  maxWindSpeedLabel = MAX(ceil(windStrength) + getRandomValue(0, 1), 1);

  char temp[16];
  snprintf(temp, 16, "%d - %d m/s", minWindSpeedLabel % 20,
           maxWindSpeedLabel % 20);
  log_debug("%s, REAL=%.1lf", temp, windStrength);
  TTF_Font* speedLabelFont = loadSmallFont(app, 30);

  // detroying old texture
  SDL_DestroyTexture(speedLabel->data.texture.texture);

  // creating new texture
  speedLabel->data.texture.texture =
      createTextTexture(app->renderer, speedLabelFont, temp, 0, 0, 200, 255);

  TTF_CloseFont(speedLabelFont);
}