#ifndef UI_HELPERS_H
#define UI_HELPERS_H

#include "../App.h"

// font loading
TTF_Font* loadMainFont(App* app, int32_t size);
TTF_Font* loadSmallFont(App* app, int32_t size);

// text creation
RenderObject* createCenteredText(App* app, const char* text, TTF_Font* font,
                                 int32_t y, SDL_Color color);
RenderObject* createCenteredTextWithTrigger(App* app, const char* text,
                                            TTF_Font* font, int32_t y,
                                            SDL_Color normalColor,
                                            SDL_Color triggeredColor,
                                            enum Button buttonName);
RenderObject* createLeftAlignedText(App* app, const char* text, TTF_Font* font,
                                    int32_t x, int32_t y, SDL_Color color);
RenderObject* createRightAlignedText(App* app, const char* text, TTF_Font* font,
                                     int32_t x, int32_t y, SDL_Color color);

// button creation
RenderObject* createCenteredButton(App* app, const char* text, TTF_Font* font,
                                   int32_t y, SDL_Color normalColor,
                                   SDL_Color triggeredColor,
                                   enum Button buttonName);
RenderObject* createBackButton(App* app, TTF_Font* font, int32_t y);

// layout helpers
int32_t getCenteredX(App* app, int32_t width);
int32_t getCenteredXForText(App* app, const char* text, TTF_Font* font);
int32_t getLeftAlignedX(App* app, int32_t width, int32_t offset);
int32_t getRightAlignedX(App* app, int32_t width, int32_t offset);

// color constants
extern const SDL_Color COLOR_WHITE;
extern const SDL_Color COLOR_GRAY;
extern const SDL_Color COLOR_RED;
extern const SDL_Color COLOR_GREEN;
extern const SDL_Color COLOR_BLUE;

// player difficulty selection
RenderObject* createPlayerDifficultyButton(App* app, const char* text,
                                           TTF_Font* font, int32_t x, int32_t y,
                                           enum Button buttonName);

#endif
