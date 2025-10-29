#include "leaderboard_render.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "../SDL/SDL_render.h"
#include "../SDL/event_handlers.h"
#include "../SDL/ui_helpers.h"
#include "log/log.h"

void renderPlayer(App* app, int32_t y, PlayerScore currPlayer,
                  const char* placeText, TTF_Font* font, uint8_t r, uint8_t g,
                  uint8_t b, uint8_t a) {
  char temp[10];
  int32_t w, h;
  int32_t x;

  SDL_Texture* playerPlace =
      createTextTexture(app->renderer, font, placeText, r, g, b, a);
  SDL_Texture* playerName =
      createTextTexture(app->renderer, font, currPlayer.name, r, g, b, a);
  sprintf(temp, "%d", currPlayer.score);
  SDL_Texture* playerScore =
      createTextTexture(app->renderer, font, temp, r, g, b, a);

  y = y * app->scalingFactorY;

  SDL_QueryTexture(playerPlace, NULL, NULL, &w, &h);
  x = (app->screenWidth / app->scalingFactorX / 2 - w) / 2 *
      app->scalingFactorX;
  w *= app->scalingFactorX;
  h *= app->scalingFactorY;
  SDL_RenderCopy(app->renderer, playerPlace, NULL, &(SDL_Rect){x, y, w, h});

  SDL_QueryTexture(playerName, NULL, NULL, &w, &h);
  x = (app->screenWidth / app->scalingFactorX - w) / 2 * app->scalingFactorX;
  w *= app->scalingFactorX;
  h *= app->scalingFactorY;
  SDL_RenderCopy(app->renderer, playerName, NULL, &(SDL_Rect){x, y, w, h});

  SDL_QueryTexture(playerScore, NULL, NULL, &w, &h);
  x = (app->screenWidth / app->scalingFactorX * 3 / 2 - w) / 2 *
      app->scalingFactorX;
  w *= app->scalingFactorX;
  h *= app->scalingFactorY;
  SDL_RenderCopy(app->renderer, playerScore, NULL, &(SDL_Rect){x, y, w, h});

  SDL_DestroyTexture(playerScore);
  SDL_DestroyTexture(playerPlace);
  SDL_DestroyTexture(playerName);
}

void loadLeaderboardToArray(PlayerScore* leaderboard) {
  char temp[256];

  char* basePath = SDL_GetBasePath();

  sprintf(temp, "%sdata/leaderboard", basePath);
  free(basePath);

  FILE* file = fopen(temp, "r");
  if (file == NULL) {
    file = fopen(temp, "w");
    for (int32_t i = 0; i < 10; ++i) {
      fprintf(file, "0 EMPTY\n");
    }
    fclose(file);
    file = fopen(temp, "r");
  }

  for (int32_t i = 0; i < 10; ++i) {
    if (fscanf(file, "%d %15s", &leaderboard[i].score, leaderboard[i].name) <
        2) {
      break;
    }
  }

  fclose(file);
}

int32_t findPlaceInLeaderboard(int32_t score) {
  char temp[256];

  char* basePath = SDL_GetBasePath();

  sprintf(temp, "%sdata/leaderboard", basePath);
  free(basePath);

  FILE* file = fopen(temp, "r");
  if (file == NULL) {
    file = fopen(temp, "w");
    for (int32_t i = 0; i < 10; ++i) {
      fprintf(file, "0 EMPTY\n");
    }
    fclose(file);
    file = fopen(temp, "r");
  }
  char name[16];
  int32_t currScore;

  for (int32_t i = 0; i < 10; ++i) {
    // broken file or smth like that
    if (fscanf(file, "%d %15s\n", &currScore, name) < 2) {
      return -1;
    }
    // if we ve found the place
    if (score > currScore) {
      fclose(file);
      return i + 1;
    }
  }
  fclose(file);
  return -1;
}

void addToLeaderBoard(const char* name, int32_t score) {
  char temp[256];

  char* basePath = SDL_GetBasePath();

  sprintf(temp, "%sdata/leaderboard", basePath);
  free(basePath);

  FILE* file = fopen(temp, "r");

  if (file == NULL) {
    file = fopen(temp, "w");
    if (file == NULL) {
      log_error("error while reading leaderboard file!\n");
      return;
    }
    for (int32_t i = 0; i < 10; ++i) {
      fprintf(file, "0 EMPTY\n");
    }
    fclose(file);
    file = fopen(temp, "r");
  }

  PlayerScore leaderboard[10];
  memset(leaderboard, 0x00, sizeof(PlayerScore) * 10);

  for (int32_t i = 0; i < 10; ++i) {
    // reading an old leaderboard table
    if (fscanf(file, "%d %15s\n", &leaderboard[i].score, leaderboard[i].name) !=
        2) {
      log_error("error reading leaderboard table");

      return;
    }
  }

  fclose(file);

  int32_t newScorePos = findPlaceInLeaderboard(score) - 1;

  if (newScorePos != -1) {
    for (int32_t i = 9; i > newScorePos; i--) {
      leaderboard[i] = leaderboard[i - 1];
    }
  }

  leaderboard[newScorePos].score = score;
  strncpy(leaderboard[newScorePos].name, name, 16);

  file = fopen(temp, "w");
  if (file == NULL) {
    log_error("error while reading leaderboard file!");
    return;
  }

  for (int32_t i = 0; i < 10; ++i) {
    fprintf(file, "%d %s\n", leaderboard[i].score, leaderboard[i].name);
  }

  fclose(file);
}

void leaderboardMain(App* app, const char* name) {
  // loading fonts
  TTF_Font* smallFont = loadSmallFont(app, 30);
  TTF_Font* mainFont = loadMainFont(app, 60);

  if (*name != '\0') {
    addToLeaderBoard(name, app->winnerScore);
  }

  // creating labels
  RenderObject* leaderboardLabel =
      createCenteredText(app, "LEADERBOARD", mainFont, 20, COLOR_WHITE);

  // creating column headers
  int32_t leftColX =
      getCenteredX(app, app->screenWidth / app->scalingFactorX / 2);
  int32_t centerColX =
      app->screenWidth / app->scalingFactorX / 2 - 20 * app->scalingFactorX;
  int32_t rightColX =
      getCenteredX(app, app->screenWidth / app->scalingFactorX / 2) +
      app->screenWidth / app->scalingFactorX / 2;

  RenderObject* placeLabel = createLeftAlignedText(app, "PLACE", smallFont,
                                                   leftColX, 150, COLOR_WHITE);
  // Recalculate X to match row positioning using logical (unscaled) coords
  int32_t tmpW, tmpH;
  SDL_QueryTexture(placeLabel->data.texture.texture, NULL, NULL, &tmpW, &tmpH);
  placeLabel->data.texture.constRect.x =
      (app->screenWidth / app->scalingFactorX / 2 - tmpW) / 2;
  placeLabel->data.texture.scaleRect.x = placeLabel->data.texture.constRect.x;
  RenderObject* nameLabel = createLeftAlignedText(app, "NAME", smallFont,
                                                  centerColX, 150, COLOR_WHITE);
  SDL_QueryTexture(nameLabel->data.texture.texture, NULL, NULL, &tmpW, &tmpH);
  nameLabel->data.texture.constRect.x =
      (app->screenWidth / app->scalingFactorX - tmpW) / 2;
  nameLabel->data.texture.scaleRect.x = nameLabel->data.texture.constRect.x;
  RenderObject* scoreLabel = createLeftAlignedText(app, "SCORE", smallFont,
                                                   rightColX, 150, COLOR_WHITE);
  SDL_QueryTexture(scoreLabel->data.texture.texture, NULL, NULL, &tmpW, &tmpH);
  scoreLabel->data.texture.constRect.x =
      (app->screenWidth / app->scalingFactorX * 3 / 2 - tmpW) / 2;
  scoreLabel->data.texture.scaleRect.x = scoreLabel->data.texture.constRect.x;

  PlayerScore leaderboardArray[10] = {0};
  loadLeaderboardToArray(leaderboardArray);

  SDL_Texture* leaderboard = SDL_CreateTexture(
      app->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
      app->screenWidth, app->screenHeight);
  SDL_SetRenderTarget(app->renderer, leaderboard);
  SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 0);
  SDL_RenderClear(app->renderer);

  renderPlayer(app, 200, leaderboardArray[0], "1st", smallFont, 179, 149, 0,
               255);
  renderPlayer(app, 250, leaderboardArray[1], "2nd", smallFont, 216, 216, 216,
               255);
  renderPlayer(app, 300, leaderboardArray[2], "3rd", smallFont, 130, 87, 44,
               255);
  renderPlayer(app, 350, leaderboardArray[3], "4th", smallFont, 128, 128, 128,
               255);
  renderPlayer(app, 400, leaderboardArray[4], "5th", smallFont, 128, 128, 128,
               255);
  renderPlayer(app, 450, leaderboardArray[5], "6th", smallFont, 128, 128, 128,
               255);
  renderPlayer(app, 500, leaderboardArray[6], "7th", smallFont, 128, 128, 128,
               255);
  renderPlayer(app, 550, leaderboardArray[7], "8th", smallFont, 128, 128, 128,
               255);
  renderPlayer(app, 600, leaderboardArray[8], "9th", smallFont, 128, 128, 128,
               255);
  renderPlayer(app, 650, leaderboardArray[9], "10th", smallFont, 128, 128, 128,
               255);

  SDL_SetRenderTarget(app->renderer, NULL);

  RenderObject* returnArrowObj = createBackButton(
      app, mainFont, leaderboardLabel->data.texture.constRect.y);

  RenderObject* objectsArr[] = {returnArrowObj, leaderboardLabel, placeLabel,
                                nameLabel, scoreLabel};

  while (app->currState == LEADERBOARD) {
    threadEventPoll(app);
    // filling up the background with black color and clearing render
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    SDL_RenderCopy(app->renderer, leaderboard, NULL, NULL);

    renderTextures(app, objectsArr, sizeof(objectsArr) / sizeof(*objectsArr),
                   SDL_TRUE);

    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
    // underscoring place label
    SDL_RenderDrawLine(
        app->renderer,
        placeLabel->data.texture.scaleRect.x - 3 * app->scalingFactorX,
        placeLabel->data.texture.scaleRect.y +
            placeLabel->data.texture.scaleRect.h + 3 * app->scalingFactorY,
        placeLabel->data.texture.scaleRect.x +
            placeLabel->data.texture.scaleRect.w,
        placeLabel->data.texture.scaleRect.y +
            placeLabel->data.texture.scaleRect.h + 3 * app->scalingFactorY);
    // underscoring score label
    SDL_RenderDrawLine(
        app->renderer,
        scoreLabel->data.texture.scaleRect.x - 3 * app->scalingFactorX,
        scoreLabel->data.texture.scaleRect.y +
            scoreLabel->data.texture.scaleRect.h + 3 * app->scalingFactorY,
        scoreLabel->data.texture.scaleRect.x +
            scoreLabel->data.texture.scaleRect.w,
        scoreLabel->data.texture.scaleRect.y +
            scoreLabel->data.texture.scaleRect.h + 3 * app->scalingFactorY);
    // underscoring name label
    SDL_RenderDrawLine(
        app->renderer,
        nameLabel->data.texture.scaleRect.x - 3 * app->scalingFactorX,
        nameLabel->data.texture.scaleRect.y +
            nameLabel->data.texture.scaleRect.h + 3 * app->scalingFactorY,
        nameLabel->data.texture.scaleRect.x +
            nameLabel->data.texture.scaleRect.w,
        nameLabel->data.texture.scaleRect.y +
            nameLabel->data.texture.scaleRect.h + 3 * app->scalingFactorY);

    // rendering
    SDL_RenderPresent(app->renderer);

    SDL_Delay(16);
  }

  freeRenderObject(leaderboardLabel);
  freeRenderObject(placeLabel);
  freeRenderObject(nameLabel);
  freeRenderObject(scoreLabel);
  freeRenderObject(returnArrowObj);
  SDL_DestroyTexture(leaderboard);
}

void leaderboardAddMain(App* app) {
  // loading fonts
  TTF_Font* smallFont = loadSmallFont(app, 30);
  TTF_Font* mainFont = loadMainFont(app, 60);

  char temp[256];
  if (app->winner == 1) {
    strcpy(temp, "BETMENT WON!!!");
    Mix_PlayChannel(-1, app->sounds[1], 0);
  } else if (app->winner == 2) {
    strcpy(temp, "JONKLER WON!!!");
    Mix_PlayChannel(-1, app->sounds[1], 0);
  } else if (app->winner == 3) {
    strcpy(temp, "ROBOCOP WON!!!");
  } else {
    strcpy(temp, "NO ONE WON!!!");
  }

  RenderObject* wonLabel =
      createCenteredText(app, temp, mainFont, 60, COLOR_GRAY);

  RenderObject* scoreLabel = NULL;
  if (app->winner != 3) {
    sprintf(temp, "SCORE: %d",
            app->winnerScore == -1337 ? 0 : app->winnerScore);
    scoreLabel = createCenteredText(app, temp, mainFont, 150, COLOR_GRAY);
  }

  RenderObject* enterNameLabel = NULL;

  RenderObject* nameInput = NULL;

  RenderObject* placeLabel = NULL;

  int32_t placeInTable = findPlaceInLeaderboard(app->winnerScore);

  // if player in the top 10 in the leaderboard
  if (placeInTable != -1) {
    enterNameLabel = createRenderObject(
        app->renderer, TEXT, 0, b_NONE, "Enter ur name:", smallFont,
        &(SDL_Point){0, 0}, &(SDL_Color){128, 128, 128, 255});
    enterNameLabel->data.texture.constRect.x =
        (app->screenWidth / app->scalingFactorX -
         enterNameLabel->data.texture.constRect.w) /
        2;
    enterNameLabel->data.texture.constRect.y = 300;

    nameInput = createRenderObject(
        app->renderer, TEXT_INPUT, 1, bTI_NAME,
        &(SDL_Rect){enterNameLabel->data.texture.constRect.x - 80,
                    enterNameLabel->data.texture.constRect.y +
                        enterNameLabel->data.texture.constRect.h,
                    400, 50},
        15, 9, smallFont);

    sprintf(temp, "YOU'RE RANKED #%d on the leaderboard", placeInTable);

    placeLabel = createRenderObject(app->renderer, TEXT, 0, b_NONE, temp,
                                    smallFont, &(SDL_Point){0, 0},
                                    &(SDL_Color){128, 128, 128, 255});
    placeLabel->data.texture.constRect.x =
        (app->screenWidth / app->scalingFactorX -
         placeLabel->data.texture.constRect.w) /
        2;
    placeLabel->data.texture.constRect.y = 250;
  }

  RenderObject* continueButton = createRenderObject(
      app->renderer, TEXT | CAN_BE_TRIGGERED, 0, b_LEADERBOARD_CONTINUE,
      "SAVE & CONTINUE", mainFont, &(SDL_Point){0, 0},
      &(SDL_Color){255, 255, 255, 255}, &(SDL_Color){230, 25, 25, 255});
  continueButton->data.texture.constRect.x =
      (app->screenWidth / app->scalingFactorX -
       continueButton->data.texture.constRect.w) /
      2;
  continueButton->data.texture.constRect.y = 670;

  RenderObject* objectsArr[] = {wonLabel,  scoreLabel, enterNameLabel,
                                nameInput, placeLabel, continueButton};

  while (app->currState == LEADERBOARD_ADD) {
    threadEventPoll(app);
    // filling up the background with black color and clearing render
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    renderTextures(app, objectsArr, sizeof(objectsArr) / sizeof(*objectsArr),
                   SDL_TRUE);

    // rendering
    SDL_RenderPresent(app->renderer);

    SDL_Delay(16);
  }

  if (placeInTable != -1) {
    strcpy(temp, nameInput->data.textInputLine.savedText);
    freeRenderObject(enterNameLabel);
    SDL_DestroyTexture(nameInput->data.textInputLine.textTexture);
    free(nameInput);
    freeRenderObject(placeLabel);
  } else {
    memset(temp, 0x00, 256);
  }

  freeRenderObject(wonLabel);
  freeRenderObject(scoreLabel);
  freeRenderObject(continueButton);

  leaderboardMain(app, temp);
}