#include "main_menu_render.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <log/log.h>

#include "../SDL/SDL_render.h"
#include "../SDL/event_handlers.h"
#include "../SDL/ui_helpers.h"
#include "../game/animations.h"
#include "../math/physics.h"
#include "log/log.h"

void mainMenu(App* app) {
  RenderObject* logoObject = createRenderObject(
      app->renderer, TEXTURE, 2, b_NONE, "media/imgs/logo.png", NULL);
  logoObject->data.texture.constRect.x =
      (app->screenWidth / app->scalingFactorX -
       logoObject->data.texture.constRect.w) /
      2;

  // inversed height
  double height = app->screenHeight / 2.;
  double velocity = 1000;

  RenderObject* objectsArr[] = {logoObject};

  // output 'dropping' logo
  while (simulateFall_1Step(&height, &velocity) != -1 &&
         app->currState != EXIT) {
    threadEventPoll(app);
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    objectsArr[0]->data.texture.constRect.y = 33 - (int32_t)height;

    renderTextures(app, objectsArr, sizeof(objectsArr) / sizeof(*objectsArr),
                   SDL_TRUE);

    SDL_RenderPresent(app->renderer);
    SDL_Delay(16);
  }

  if (app->currState == EXIT) {
    freeRenderObject(logoObject);
    SDL_Cleanup(app);
    exit(0);
  }

  // setting up current state to MENU
  app->currState = MENU;

  log_info("success completed state: LOGO");

  log_info("current state: MENU");

  // func will wait untill user do some interactions
  menuRenderLoop(app, logoObject);

  log_info("success completed state: MENU");

  // going back to mainLoop
}

void menuRenderLoop(App* app, RenderObject* logo) {
  // loading main font
  TTF_Font* menuButtonsFont = loadMainFont(app, 80);

  // create icon buttons
  int32_t iconY = 6 + ((!app->settings.isFullscreen) ? 24 : 0);
  RenderObject* infoButtonObj = createRenderObject(
      app->renderer, TEXTURE | CAN_BE_TRIGGERED, 1, b_HELP,
      "media/imgs/helpIco.png",
      &(SDL_Point){app->screenWidth / app->scalingFactorX - 55, iconY});

  RenderObject* leaderboardObj = createRenderObject(
      app->renderer, TEXTURE | CAN_BE_TRIGGERED, 1, b_LEADERBOARDS,
      "media/imgs/trophy.png",
      &(SDL_Point){app->screenWidth / app->scalingFactorX - 55, iconY + 60});

  // create menu buttons
  RenderObject* playTextObj = createCenteredButton(
      app, "PLAY", menuButtonsFont, 404, COLOR_GRAY, COLOR_RED, b_PLAY);

  // calculate position for settings button
  int32_t settingsY = 404 + playTextObj->data.texture.constRect.h;
  RenderObject* settingsTextObj =
      createCenteredButton(app, "SETTINGS", menuButtonsFont, settingsY,
                           COLOR_GRAY, COLOR_RED, b_SETTINGS);

  // calculate position for quit button
  int32_t quitY = settingsY + settingsTextObj->data.texture.constRect.h;
  RenderObject* quitTextObj = createCenteredButton(
      app, "QUIT", menuButtonsFont, quitY, COLOR_GRAY, COLOR_RED, b_QUIT);
  // creting objects arr
  RenderObject* objectsArr[] = {leaderboardObj,  infoButtonObj, playTextObj,
                                settingsTextObj, quitTextObj,   logo};

  uint32_t rgbOffset = 0;
  while (app->currState == MENU) {
    threadEventPoll(app);
    // filling up the background with black color and clearing render
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    renderTextures(app, objectsArr, sizeof(objectsArr) / sizeof(*objectsArr),
                   SDL_TRUE);

    // output rgb lines (1 reversed)
    drawRGBLine(app, 390 * app->scalingFactorY, rgbOffset, SDL_FALSE);
    drawRGBLine(app, 700 * app->scalingFactorY, rgbOffset, SDL_TRUE);

    // rendering
    SDL_RenderPresent(app->renderer);
    rgbOffset += 4;

    SDL_Delay(16);
  }

  // closing font
  TTF_CloseFont(menuButtonsFont);
  //

  // destroying textures
  freeRenderObject(playTextObj);
  freeRenderObject(settingsTextObj);
  freeRenderObject(quitTextObj);
  freeRenderObject(logo);
  freeRenderObject(infoButtonObj);
  freeRenderObject(leaderboardObj);
  //
}