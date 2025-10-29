#include "help_render.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include "../SDL/SDL_render.h"
#include "../SDL/event_handlers.h"
#include "../SDL/ui_helpers.h"

void helpMain(App* app) {
  // load fonts
  TTF_Font* titleFont = loadMainFont(app, 60);
  TTF_Font* rulesFont = loadMainFont(app, 20);
  TTF_Font* aboutFont = loadMainFont(app, 20);

  // create title and back button
  RenderObject* rulesTextObj =
      createCenteredText(app, "RULES", titleFont, 24 + 5, COLOR_WHITE);
  RenderObject* returnArrowObj =
      createBackButton(app, titleFont, rulesTextObj->data.texture.constRect.y);

  // rules text
  RenderObject* rulesTextObj_1 = createRenderObject(
      app->renderer, TEXT, 1, b_NONE,
      "1) Two players take part in the game, each of them takes turns firing a",
      rulesFont, &(SDL_Point){10, 35 + rulesTextObj->data.texture.constRect.h},
      &(SDL_Color){255, 255, 255, 255});
  RenderObject* rulesTextObj_2 = createRenderObject(
      app->renderer, TEXT, 1, b_NONE,
      "certain weapon. The list of weapons available for use can be selected",
      rulesFont,
      &(SDL_Point){10, rulesTextObj_1->data.texture.constRect.y +
                           rulesTextObj_1->data.texture.constRect.h},
      &(SDL_Color){255, 255, 255, 255});
  RenderObject* rulesTextObj_3 = createRenderObject(
      app->renderer, TEXT, 1, b_NONE, "in the settings.", rulesFont,
      &(SDL_Point){10, rulesTextObj_2->data.texture.constRect.y +
                           rulesTextObj_2->data.texture.constRect.h},
      &(SDL_Color){255, 255, 255, 255});
  RenderObject* rulesTextObj_4 = createRenderObject(
      app->renderer, TEXT, 1, b_NONE,
      "2) When making a shot, the player sets both the force of the shot and ",
      rulesFont,
      &(SDL_Point){10, 15 + rulesTextObj_3->data.texture.constRect.y +
                           rulesTextObj_3->data.texture.constRect.h},
      &(SDL_Color){255, 255, 255, 255});
  RenderObject* rulesTextObj_5 = createRenderObject(
      app->renderer, TEXT, 1, b_NONE,
      "the angle of inclination. The number of shots is limited by the number ",
      rulesFont,
      &(SDL_Point){10, rulesTextObj_4->data.texture.constRect.y +
                           rulesTextObj_4->data.texture.constRect.h},
      &(SDL_Color){255, 255, 255, 255});
  RenderObject* rulesTextObj_6 = createRenderObject(
      app->renderer, TEXT, 1, b_NONE, "of weapons (12 weapons).", rulesFont,
      &(SDL_Point){10, rulesTextObj_5->data.texture.constRect.y +
                           rulesTextObj_5->data.texture.constRect.h},
      &(SDL_Color){255, 255, 255, 255});
  RenderObject* rulesTextObj_7 = createRenderObject(
      app->renderer, TEXT, 1, b_NONE,
      "3) During the whole game each player can move his tank only 4 times, ",
      rulesFont,
      &(SDL_Point){10, 15 + rulesTextObj_6->data.texture.constRect.y +
                           rulesTextObj_6->data.texture.constRect.h},
      &(SDL_Color){255, 255, 255, 255});
  RenderObject* rulesTextObj_8 = createRenderObject(
      app->renderer, TEXT, 1, b_NONE,
      "and for a limited distance. Also the tank can not pass on too high ",
      rulesFont,
      &(SDL_Point){10, rulesTextObj_7->data.texture.constRect.y +
                           rulesTextObj_7->data.texture.constRect.h},
      &(SDL_Color){255, 255, 255, 255});
  RenderObject* rulesTextObj_9 = createRenderObject(
      app->renderer, TEXT, 1, b_NONE, "slopes", rulesFont,
      &(SDL_Point){10, rulesTextObj_8->data.texture.constRect.y +
                           rulesTextObj_8->data.texture.constRect.h},
      &(SDL_Color){255, 255, 255, 255});

  // about
  RenderObject* aboutTextObj =
      createRenderObject(app->renderer, TEXT, 1, b_NONE, "ABOUT", titleFont,
                         &(SDL_Point){0, 0}, &(SDL_Color){255, 255, 255, 255});
  aboutTextObj->data.texture.constRect.x =
      (app->screenWidth / app->scalingFactorX -
       aboutTextObj->data.texture.constRect.w) /
      2;
  aboutTextObj->data.texture.constRect.y =
      (10 + rulesTextObj_9->data.texture.constRect.y +
       rulesTextObj_9->data.texture.constRect.h);

  // about text
  RenderObject* aboutTextObj_1 = createRenderObject(
      app->renderer, TEXT, 1, b_NONE,
      "Разработчики: Митин А.А., Богданов Д.А. (с) 2025", aboutFont,
      &(SDL_Point){10, aboutTextObj->data.texture.constRect.y +
                           aboutTextObj->data.texture.constRect.h + 5},
      &(SDL_Color){255, 255, 255, 255});
  RenderObject* aboutTextObj_2 = createRenderObject(
      app->renderer, TEXT, 1, b_NONE, "Группа: 5131001/40001", aboutFont,
      &(SDL_Point){10, aboutTextObj_1->data.texture.constRect.y +
                           aboutTextObj_1->data.texture.constRect.h + 5},
      &(SDL_Color){255, 255, 255, 255});
  RenderObject* aboutTextObj_3 = createRenderObject(
      app->renderer, TEXT, 1, b_NONE,
      "ВУЗ: Санкт-Петербургский политехнический университет Петра Великого",
      aboutFont,
      &(SDL_Point){10, aboutTextObj_2->data.texture.constRect.y +
                           aboutTextObj_2->data.texture.constRect.h + 5},
      &(SDL_Color){255, 255, 255, 255});
  RenderObject* aboutTextObj_4 = createRenderObject(
      app->renderer, TEXT, 1, b_NONE,
      "Институт: Институт компьютерных наук и кибербезопасности (ИКНК)",
      aboutFont,
      &(SDL_Point){10, aboutTextObj_3->data.texture.constRect.y +
                           aboutTextObj_3->data.texture.constRect.h + 5},
      &(SDL_Color){255, 255, 255, 255});
  RenderObject* aboutTextObj_5 = createRenderObject(
      app->renderer, TEXT, 1, b_NONE, "Высшая школа кибербезопасности",
      aboutFont,
      &(SDL_Point){10, aboutTextObj_4->data.texture.constRect.y +
                           aboutTextObj_4->data.texture.constRect.h + 5},
      &(SDL_Color){255, 255, 255, 255});

  RenderObject* objectsArr[] = {rulesTextObj_1, rulesTextObj_2, rulesTextObj_3,
                                returnArrowObj, rulesTextObj_4, rulesTextObj_5,
                                rulesTextObj_6, rulesTextObj_7, rulesTextObj_8,
                                rulesTextObj_9, rulesTextObj,   aboutTextObj,
                                aboutTextObj_1, aboutTextObj_2, aboutTextObj_3,
                                aboutTextObj_4, aboutTextObj_5};

  SDL_Texture* textTexture = SDL_CreateTexture(
      app->renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_TARGET,
      app->screenWidth, app->screenHeight);
  SDL_SetRenderTarget(app->renderer, textTexture);
  SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 0);
  SDL_RenderClear(app->renderer);

  // rendering all text on a new texture
  // so after that we just need to render that texture
  renderTextures(app, objectsArr, sizeof(objectsArr) / sizeof(*objectsArr),
                 SDL_TRUE);

  SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);

  SDL_RenderDrawLine(
      app->renderer, 0,
      (32 + rulesTextObj->data.texture.constRect.h) * app->scalingFactorY,
      app->screenWidth,
      (32 + rulesTextObj->data.texture.constRect.h) * app->scalingFactorY);

  SDL_RenderDrawLine(app->renderer, 0,
                     (10 + rulesTextObj_9->data.texture.constRect.y +
                      rulesTextObj_9->data.texture.constRect.h) *
                         app->scalingFactorY,
                     app->screenWidth,
                     (10 + rulesTextObj_9->data.texture.constRect.y +
                      rulesTextObj_9->data.texture.constRect.h) *
                         app->scalingFactorY);

  SDL_RenderDrawLine(app->renderer, 0,
                     (aboutTextObj->data.texture.constRect.y +
                      aboutTextObj->data.texture.constRect.h) *
                         app->scalingFactorY,
                     app->screenWidth,
                     (aboutTextObj->data.texture.constRect.y +
                      aboutTextObj->data.texture.constRect.h) *
                         app->scalingFactorY);

  SDL_RenderPresent(app->renderer);
  SDL_SetRenderTarget(app->renderer, NULL);

  freeRenderObject(rulesTextObj);
  freeRenderObject(rulesTextObj_1);
  freeRenderObject(rulesTextObj_2);
  freeRenderObject(rulesTextObj_3);
  freeRenderObject(rulesTextObj_4);
  freeRenderObject(rulesTextObj_5);
  freeRenderObject(rulesTextObj_6);
  freeRenderObject(rulesTextObj_7);
  freeRenderObject(rulesTextObj_8);
  freeRenderObject(rulesTextObj_9);
  freeRenderObject(aboutTextObj);
  freeRenderObject(aboutTextObj_1);
  freeRenderObject(aboutTextObj_2);
  freeRenderObject(aboutTextObj_3);
  freeRenderObject(aboutTextObj_4);
  freeRenderObject(aboutTextObj_5);

  objectsArr[0] = returnArrowObj;
  while (app->currState == HELP) {
    threadEventPoll(app);
    // filling up the background with black color and clearing render
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    SDL_RenderCopy(app->renderer, textTexture, NULL, NULL);

    renderTextures(app, objectsArr, 1, SDL_TRUE);

    // rendering
    SDL_RenderPresent(app->renderer);

    SDL_Delay(16);
  }

  freeRenderObject(returnArrowObj);
  SDL_DestroyTexture(textTexture);

  TTF_CloseFont(titleFont);
  TTF_CloseFont(rulesFont);
  TTF_CloseFont(aboutFont);
}