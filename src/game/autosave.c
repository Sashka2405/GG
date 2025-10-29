#include "autosave.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "log/log.h"

void clearSave(App* app) {
  char temp[256];

  sprintf(temp, "%sdata/autosave", app->basePath);
  FILE* file = fopen(temp, "w");
  if (file == NULL) {
    log_error("error while creating saves/autosave file");
    return;
  }
  fclose(file);
}

void saveCurrentState(App* app, Player* firstPlayer, Player* secondPlayer,
                      int32_t* heightMap, int32_t isFirstTarget,
                      uint32_t seed) {
  char temp[256];

  sprintf(temp, "%sdata/autosave", app->basePath);
  FILE* file = fopen(temp, "w");
  if (file == NULL) {
    log_error("error while creating saves/autosave file");
    return;
  }

  fprintf(file, "please DONT change anything in this file thank you!!! <3\n");

  fprintf(file, "tp:%d\n", app->timesPlayed);

  fprintf(file, "w:%d\n", app->screenWidth);
  fprintf(file, "h:%d\n", app->screenHeight);
  fprintf(file, "seed:%u\n", seed);

  fprintf(file, "tar:%d\n", isFirstTarget);

  fprintf(file, "t1s:%d\n", firstPlayer->score);
  fprintf(file, "t1x:%d\n", firstPlayer->tankObj->data.texture.constRect.x);
  fprintf(file, "t1t:%d\n", firstPlayer->type);
  fprintf(file, "t1ml:%d\n", firstPlayer->movesLeft);
  fprintf(file, "t1ga:%lf\n", firstPlayer->gunAngle);
  fprintf(file, "t1gp:%d\n", firstPlayer->firingPower);

  fprintf(file, "t2s:%d\n", secondPlayer->score);
  fprintf(file, "t2x:%d\n", secondPlayer->tankObj->data.texture.constRect.x);
  fprintf(file, "t2t:%d\n", secondPlayer->type);
  fprintf(file, "t2ml:%d\n", secondPlayer->movesLeft);
  fprintf(file, "t2ga:%lf\n", secondPlayer->gunAngle);
  fprintf(file, "t2gp:%d\n", secondPlayer->firingPower);

  fprintf(file, "hm:%d\n", app->screenWidth);
  for (int32_t i = 0; i < app->screenWidth; ++i) {
    fprintf(file, "%d\n", heightMap[i]);
  }

  fclose(file);
}

// func return 1 if save is broken or doesn't exist
//      return 2 if save resolution doesn't match current
int32_t loadSavedState(App* app, Player* firstPlayer, Player* secondPlayer,
                       int32_t* heightMap, uint32_t* seed) {
  char strTemp[256];

  sprintf(strTemp, "%sdata/autosave", app->basePath);

  FILE* file = fopen(strTemp, "r");
  if (file == NULL) {
    log_error("error reading saves/autosave");

    return 1;
  }

  // skipping first line
  char c;
  while ((c = fgetc(file)) != '\n' && c != EOF);

  if (!fscanf(file, "tp:%d\n", &app->timesPlayed)) return 1;

  int32_t w, h;
  if (!fscanf(file, "w:%d\n", &w)) return 1;
  if (!fscanf(file, "h:%d\n", &h)) return 1;
  if (w != app->screenWidth || h != app->screenHeight) {
    log_error("incorrect screen resolution");

    return 2;
  }

  if (!fscanf(file, "seed:%u\n", seed)) return 1;

  int32_t temp;
  if (!fscanf(file, "tar:%d\n", &temp)) return 1;
  if (temp) {
    app->currPlayer = firstPlayer;
  } else {
    app->currPlayer = secondPlayer;
  }

  if (!fscanf(file, "t1s:%d\n", &temp)) return 1;
  firstPlayer->score = temp;

  if (!fscanf(file, "t1x:%d\n", &temp)) return 1;
  firstPlayer->x = temp;

  double tempDouble;

  if (!fscanf(file, "t1t:%d\n", &temp)) return 1;
  firstPlayer->type = temp;

  if (!fscanf(file, "t1ml:%d\n", &temp)) return 1;
  firstPlayer->movesLeft = temp;

  if (!fscanf(file, "t1ga:%lf\n", &tempDouble)) return 1;
  firstPlayer->gunAngle = tempDouble;

  if (!fscanf(file, "t1gp:%d\n", &temp)) return 1;
  firstPlayer->firingPower = temp;

  if (!fscanf(file, "t2s:%d\n", &temp)) return 1;
  secondPlayer->score = temp;

  if (!fscanf(file, "t2x:%d\n", &temp)) return 1;
  secondPlayer->x = temp;

  if (!fscanf(file, "t2t:%d\n", &temp)) return 1;
  secondPlayer->type = temp;

  if (!fscanf(file, "t2ml:%d\n", &temp)) return 1;
  secondPlayer->movesLeft = temp;

  if (!fscanf(file, "t2ga:%lf\n", &tempDouble)) return 1;
  secondPlayer->gunAngle = tempDouble;

  if (!fscanf(file, "t2gp:%d\n", &temp)) return 1;
  secondPlayer->firingPower = temp;

  if (!fscanf(file, "hm:%d\n", &temp)) return 1;

  if (temp != app->screenWidth) {
    return 2;
  }

  for (int32_t i = 0; i < temp; ++i) {
    if (!fscanf(file, "%d\n", heightMap + i)) return 1;
  }

  fclose(file);
  return 0;
}