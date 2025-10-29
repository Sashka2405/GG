#include "ui_helpers.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include "SDL_render.h"

// color constants
const SDL_Color COLOR_WHITE = {255, 255, 255, 255};
const SDL_Color COLOR_GRAY = {128, 128, 128, 255};
const SDL_Color COLOR_RED = {230, 25, 25, 255};
const SDL_Color COLOR_GREEN = {0, 168, 107, 255};
const SDL_Color COLOR_BLUE = {0, 216, 255, 255};

// fonts
TTF_Font* loadMainFont(App* app, int32_t size) {
  char temp[256];
  sprintf(temp, "%smedia/fonts/PixeloidSans-Bold.ttf", app->basePath);
  return loadFont(temp, size);
}

TTF_Font* loadSmallFont(App* app, int32_t size) {
  char temp[256];
  sprintf(temp, "%smedia/fonts/PixeloidSans.ttf", app->basePath);
  return loadFont(temp, size);
}

// layout
int32_t getCenteredX(App* app, int32_t width) {
  return (app->screenWidth / app->scalingFactorX - width) / 2;
}

int32_t getCenteredXForText(App* app, const char* text, TTF_Font* font) {
  int32_t w, h;
  TTF_SizeText(font, text, &w, &h);
  return getCenteredX(app, w);
}

int32_t getLeftAlignedX(App* app, int32_t width, int32_t offset) {
  (void)app;
  (void)width;
  return offset;
}

int32_t getRightAlignedX(App* app, int32_t width, int32_t offset) {
  return app->screenWidth / app->scalingFactorX - width - offset;
}

// text fields
RenderObject* createCenteredText(App* app, const char* text, TTF_Font* font,
                                 int32_t y, SDL_Color color) {
  RenderObject* textObj = createRenderObject(
      app->renderer, TEXT, 1, b_NONE, text, font, &(SDL_Point){0, y}, &color);

  textObj->data.texture.constRect.x = getCenteredXForText(app, text, font);
  return textObj;
}

RenderObject* createCenteredTextWithTrigger(App* app, const char* text,
                                            TTF_Font* font, int32_t y,
                                            SDL_Color normalColor,
                                            SDL_Color triggeredColor,
                                            enum Button buttonName) {
  RenderObject* textObj = createRenderObject(
      app->renderer, TEXT | CAN_BE_TRIGGERED, 1, buttonName, text, font,
      &(SDL_Point){0, y}, &normalColor, &triggeredColor);

  textObj->data.texture.constRect.x = getCenteredXForText(app, text, font);
  return textObj;
}

RenderObject* createLeftAlignedText(App* app, const char* text, TTF_Font* font,
                                    int32_t x, int32_t y, SDL_Color color) {
  return createRenderObject(app->renderer, TEXT, 1, b_NONE, text, font,
                            &(SDL_Point){x, y}, &color);
}

RenderObject* createRightAlignedText(App* app, const char* text, TTF_Font* font,
                                     int32_t x, int32_t y, SDL_Color color) {
  int32_t w, h;
  TTF_SizeText(font, text, &w, &h);
  return createRenderObject(app->renderer, TEXT, 1, b_NONE, text, font,
                            &(SDL_Point){x - w, y}, &color);
}

// buttons
RenderObject* createCenteredButton(App* app, const char* text, TTF_Font* font,
                                   int32_t y, SDL_Color normalColor,
                                   SDL_Color triggeredColor,
                                   enum Button buttonName) {
  return createCenteredTextWithTrigger(app, text, font, y, normalColor,
                                       triggeredColor, buttonName);
}

RenderObject* createBackButton(App* app, TTF_Font* font, int32_t y) {
  RenderObject* backButton = createRenderObject(
      app->renderer, TEXT | CAN_BE_TRIGGERED, 1, b_SETTINGS_BACK, "<", font,
      &(SDL_Point){20, y}, &COLOR_WHITE, &COLOR_RED);

  // Center vertically relative to the title
  backButton->data.texture.constRect.y = y;
  return backButton;
}

// Player difficulty selection helpers
RenderObject* createPlayerDifficultyButton(App* app, const char* text,
                                           TTF_Font* font, int32_t x, int32_t y,
                                           enum Button buttonName) {
  RenderObject* button = createRenderObject(
      app->renderer, TEXT | CAN_BE_TRIGGERED, 1, buttonName, text, font,
      &(SDL_Point){0, y}, &(SDL_Color){230, 230, 230, 255}, &COLOR_RED);

  button->data.texture.constRect.x = x;
  return button;
}
