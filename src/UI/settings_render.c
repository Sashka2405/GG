#include "settings_render.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include "../SDL/SDL_render.h"
#include "../SDL/event_handlers.h"
#include "../SDL/ui_helpers.h"
#include "../game/settings.h"
#include "log/log.h"

void settingsMain(App* app) {
  Mix_VolumeMusic(app->settings.currentVolume);  // set volume

  // load fonts
  TTF_Font* mainFont = loadMainFont(app, 40);
  TTF_Font* titleFont = loadMainFont(app, 50);
  if (!mainFont || !titleFont) {
    log_error("error loading fonts");
    return;
  }

  // create title
  RenderObject* settingsTextObj =
      createCenteredText(app, "SETTINGS", titleFont, 20, COLOR_WHITE);

  // create section labels
  int32_t leftSectionX =
      getCenteredX(app, app->screenWidth / app->scalingFactorX / 2);
  int32_t rightSectionX =
      getCenteredX(app, app->screenWidth / app->scalingFactorX / 2) +
      app->screenWidth / app->scalingFactorX / 2;

  RenderObject* videoTextObj = createLeftAlignedText(
      app, "VIDEO", mainFont, leftSectionX, 100, COLOR_WHITE);
  RenderObject* soundTextObj = createLeftAlignedText(
      app, "SOUND", mainFont, leftSectionX,
      app->screenHeight / app->scalingFactorY / 3 + 10, COLOR_WHITE);
  RenderObject* weaponsTextObj = createLeftAlignedText(
      app, "WEAPONS", mainFont, rightSectionX, 100, COLOR_WHITE);

  // Center headers within their halves
  int32_t wTmp, hTmp;
  int32_t leftCenterX = app->screenWidth / app->scalingFactorX / 4;
  int32_t rightCenterX = app->screenWidth / app->scalingFactorX * 3 / 4;

  SDL_QueryTexture(videoTextObj->data.texture.texture, NULL, NULL, &wTmp,
                   &hTmp);
  videoTextObj->data.texture.constRect.x = leftCenterX - wTmp / 2;

  SDL_QueryTexture(soundTextObj->data.texture.texture, NULL, NULL, &wTmp,
                   &hTmp);
  soundTextObj->data.texture.constRect.x = leftCenterX - wTmp / 2;

  SDL_QueryTexture(weaponsTextObj->data.texture.texture, NULL, NULL, &wTmp,
                   &hTmp);
  weaponsTextObj->data.texture.constRect.x = rightCenterX - wTmp / 2;

  // volume label
  int32_t volumeY = soundTextObj->data.texture.constRect.y +
                    soundTextObj->data.texture.constRect.h + 10;
  RenderObject* volumeTextObj = createLeftAlignedText(
      app, "Volume:", mainFont, leftSectionX, volumeY, COLOR_GRAY);
  SDL_QueryTexture(volumeTextObj->data.texture.texture, NULL, NULL, &wTmp,
                   &hTmp);
  volumeTextObj->data.texture.constRect.x = leftCenterX - wTmp / 2;

  // back button
  RenderObject* returnArrowObj = createBackButton(
      app, titleFont, settingsTextObj->data.texture.constRect.y);

  // volume slider
  RenderObject* volumeSliderObj = createRenderObject(
      app->renderer, SLIDER, 1, s_VOLUME,
      &(SDL_Rect){.x = 30,
                  .y = volumeTextObj->data.texture.constRect.y +
                       volumeTextObj->data.texture.constRect.h + 20,
                  .w = 300,
                  .h = 50});
  // Center slider within left half
  volumeSliderObj->data.texture.constRect.x =
      leftCenterX - volumeSliderObj->data.texture.constRect.w / 2;

  // volume increment/decrement triangles
  RenderObject* volumeDecrTriangleObj = createRenderObject(
      app->renderer, TRIANGLE | CAN_BE_TRIGGERED, 1, bT_DECREMENT_VOLUME,
      &(SDL_Point){volumeSliderObj->data.texture.constRect.x - 30,
                   volumeSliderObj->data.texture.constRect.y +
                       volumeSliderObj->data.texture.constRect.h / 2},
      &(SDL_Point){volumeSliderObj->data.texture.constRect.x - 5,
                   volumeSliderObj->data.texture.constRect.y + 10},
      &(SDL_Point){volumeSliderObj->data.texture.constRect.x - 5,
                   volumeSliderObj->data.texture.constRect.y +
                       volumeSliderObj->data.texture.constRect.h - 10},
      &(SDL_Color){128, 128, 128, 255}, &(SDL_Color){230, 25, 25, 255});

  RenderObject* volumeIncrTriangleObj = createRenderObject(
      app->renderer, TRIANGLE | CAN_BE_TRIGGERED, 1, bT_INCREMENT_VOLUME,
      &(SDL_Point){volumeSliderObj->data.texture.constRect.x +
                       volumeSliderObj->data.texture.constRect.w + 30,
                   volumeSliderObj->data.texture.constRect.y +
                       volumeSliderObj->data.texture.constRect.h / 2},
      &(SDL_Point){volumeSliderObj->data.texture.constRect.x +
                       volumeSliderObj->data.texture.constRect.w + 5,
                   volumeSliderObj->data.texture.constRect.y + 10},
      &(SDL_Point){volumeSliderObj->data.texture.constRect.x +
                       volumeSliderObj->data.texture.constRect.w + 5,
                   volumeSliderObj->data.texture.constRect.y +
                       volumeSliderObj->data.texture.constRect.h - 10},
      &(SDL_Color){128, 128, 128, 255}, &(SDL_Color){230, 25, 25, 255});

  //

  RenderObject* smallBulletLabel = createRenderObject(
      app->renderer, TEXT | CAN_BE_TRIGGERED, 1, b_W1, "small bullet", mainFont,
      &(SDL_Point){0, 0}, &(SDL_Color){128, 128, 128, 255},
      &(SDL_Color){0, 216, 255, 255});
  smallBulletLabel->data.texture.constRect.x =
      (app->screenWidth / app->scalingFactorX * 3 / 2 -
       smallBulletLabel->data.texture.constRect.w) /
      2;
  smallBulletLabel->data.texture.constRect.y = 175;

  RenderObject* bigBulletLabel = createRenderObject(
      app->renderer, TEXT | CAN_BE_TRIGGERED, 1, b_W2, "BIG BULLET", mainFont,
      &(SDL_Point){0, 0}, &(SDL_Color){128, 128, 128, 255},
      &(SDL_Color){0, 100, 255, 255});
  bigBulletLabel->data.texture.constRect.x =
      (app->screenWidth / app->scalingFactorX * 3 / 2 -
       bigBulletLabel->data.texture.constRect.w) /
      2;
  bigBulletLabel->data.texture.constRect.y = 250;

  RenderObject* smallBoomLabel = createRenderObject(
      app->renderer, TEXT | CAN_BE_TRIGGERED, 1, b_W3, "small boom", mainFont,
      &(SDL_Point){0, 0}, &(SDL_Color){128, 128, 128, 255},
      &(SDL_Color){230, 25, 25, 255});
  smallBoomLabel->data.texture.constRect.x =
      (app->screenWidth / app->scalingFactorX * 3 / 2 -
       smallBoomLabel->data.texture.constRect.w) /
      2;
  smallBoomLabel->data.texture.constRect.y = 325;

  RenderObject* bigBoomLabel = createRenderObject(
      app->renderer, TEXT | CAN_BE_TRIGGERED, 1, b_W4, "BIG BOOM", mainFont,
      &(SDL_Point){0, 0}, &(SDL_Color){128, 128, 128, 255},
      &(SDL_Color){248, 0, 0, 255});
  bigBoomLabel->data.texture.constRect.x =
      (app->screenWidth / app->scalingFactorX * 3 / 2 -
       bigBoomLabel->data.texture.constRect.w) /
      2;
  bigBoomLabel->data.texture.constRect.y = 400;

  // checkbox textures
  SDL_Texture* checkboxText =
      createTextTexture(app->renderer, mainFont, "[ ]", 128, 128, 128,
                        255);  // checkbox (unchecked)
  SDL_Texture* checkboxCheckedText =
      createTextTexture(app->renderer, mainFont, "[X]", 128, 128, 128,
                        255);  // checkbox (checked)

  // surfaces for bttns
  SDL_Rect checkboxRect;
  //
  SDL_QueryTexture(checkboxText, NULL, NULL, &checkboxRect.w, &checkboxRect.h);

  // Fullscreen label
  RenderObject* fullscreenTextObj = createRenderObject(
      app->renderer, TEXT, 1, b_NONE, "Fullscreen", mainFont,
      &(SDL_Point){30, 20}, &(SDL_Color){128, 128, 128, 255});

  SDL_QueryTexture(fullscreenTextObj->data.texture.texture, NULL, NULL, &wTmp,
                   &hTmp);
  fullscreenTextObj->data.texture.constRect.x =
      leftCenterX - wTmp / 2 - checkboxRect.w / 2;
  fullscreenTextObj->data.texture.constRect.y =
      videoTextObj->data.texture.constRect.y +
      videoTextObj->data.texture.constRect.h + 10;
  //

  checkboxRect.x = fullscreenTextObj->data.texture.constRect.x +
                   fullscreenTextObj->data.texture.constRect.w + 10;
  checkboxRect.y = fullscreenTextObj->data.texture.constRect.y;

  RenderObject* controlsText = createRenderObject(
      app->renderer, TEXT, 1, b_NONE, "CONTROLS", mainFont,
      &(SDL_Point){0, app->screenHeight / app->scalingFactorY / 3 * 2 + 5},
      &(SDL_Color){255, 255, 255, 255});
  controlsText->data.texture.constRect.x =
      (app->screenWidth / app->scalingFactorX -
       controlsText->data.texture.constRect.w) /
      2;

  RenderObject* controls1Text = createRenderObject(
      app->renderer, TEXT, 1, b_NONE, "W/S: change power   ↑/↓: change angle",
      mainFont,
      &(SDL_Point){10, controlsText->data.texture.constRect.y +
                           controlsText->data.texture.constRect.h + 5},
      &(SDL_Color){128, 128, 128, 255});

  RenderObject* controls2Text = createRenderObject(
      app->renderer, TEXT, 1, b_NONE, "←/→: move tank          space: shoot",
      mainFont,
      &(SDL_Point){10, controls1Text->data.texture.constRect.y +
                           controls1Text->data.texture.constRect.h + 5},
      &(SDL_Color){128, 128, 128, 255});

  RenderObject checkboxFullscreen = {
      .data.texture.texture =
          app->settings.isFullscreen ? checkboxCheckedText : checkboxText,
      .data.texture.triggeredTexture =
          app->settings.isFullscreen ? checkboxCheckedText : checkboxText,
      .data.texture.constRect = checkboxRect,
      .data.texture.scaleRect = checkboxRect,
      .canBeTriggered = SDL_TRUE,
      .Zpos = 1,
      .buttonName = b_CHECKBOX_FULLSCREEN,
  };

  // render arr
  RenderObject* objectsArr[] = {
      &checkboxFullscreen,   soundTextObj,          settingsTextObj,
      returnArrowObj,        smallBulletLabel,      bigBulletLabel,
      smallBoomLabel,        bigBoomLabel,          weaponsTextObj,
      fullscreenTextObj,     videoTextObj,          volumeSliderObj,
      volumeIncrTriangleObj, volumeDecrTriangleObj, volumeTextObj,
      controlsText,          controls1Text,         controls2Text};

  int32_t prevVolume = app->settings.currentVolume;

  while (app->currState == SETTINGS) {
    threadEventPoll(app);
    // new texture if volume changed
    if (app->settings.currentVolume != prevVolume) {
      Mix_VolumeMusic(app->settings.currentVolume);
      prevVolume = app->settings.currentVolume;
    }

    // clear screen
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    // draw dividing lines
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
    // vertical line in the middle
    SDL_RenderDrawLine(app->renderer, app->screenWidth / 2,
                       (settingsTextObj->data.texture.constRect.y +
                        settingsTextObj->data.texture.constRect.h + 10) *
                           app->scalingFactorY,
                       app->screenWidth / 2, app->screenHeight * 2. / 3);
    // horizontal line under the title
    SDL_RenderDrawLine(app->renderer, 0,
                       (settingsTextObj->data.texture.constRect.y +
                        settingsTextObj->data.texture.constRect.h + 10) *
                           app->scalingFactorY,
                       app->screenWidth,
                       (settingsTextObj->data.texture.constRect.y +
                        settingsTextObj->data.texture.constRect.h + 10) *
                           app->scalingFactorY);
    // horizontal line in the left part n1
    SDL_RenderDrawLine(app->renderer, 0, app->screenHeight / 3,
                       app->screenWidth / 2, app->screenHeight / 3);
    // horizontal line in the left part n2
    SDL_RenderDrawLine(app->renderer, 0, app->screenHeight / 3 * 2,
                       app->screenWidth, app->screenHeight / 3 * 2);

    if (!app->settings.isFullscreen) {
      objectsArr[0]->data.texture.texture =
          objectsArr[0]->data.texture.triggeredTexture = checkboxText;
    } else {
      objectsArr[0]->data.texture.texture =
          objectsArr[0]->data.texture.triggeredTexture = checkboxCheckedText;
    }

    renderTextures(app, objectsArr, sizeof(objectsArr) / sizeof(*objectsArr),
                   SDL_TRUE);

    SDL_RenderPresent(app->renderer);
    SDL_Delay(16);
  }

  // clear bttns if exit
  freeRenderObject(soundTextObj);
  freeRenderObject(settingsTextObj);
  freeRenderObject(returnArrowObj);
  freeRenderObject(weaponsTextObj);
  freeRenderObject(fullscreenTextObj);
  freeRenderObject(videoTextObj);
  freeRenderObject(volumeSliderObj);
  freeRenderObject(volumeTextObj);
  freeRenderObject(smallBulletLabel);
  freeRenderObject(bigBulletLabel);
  freeRenderObject(smallBoomLabel);
  freeRenderObject(bigBoomLabel);
  freeRenderObject(controls1Text);
  freeRenderObject(controls2Text);

  SDL_DestroyTexture(checkboxCheckedText);
  SDL_DestroyTexture(checkboxText);

  TTF_CloseFont(mainFont);
  TTF_CloseFont(titleFont);

  // saving settings
  saveSettings(app);
}