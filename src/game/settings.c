#include "settings.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "log/log.h"

#ifdef _WIN32
#include <windows.h>
#endif

uint8_t createFolder(char* name, uint32_t mode) {
#if defined(__unix__)
  return mkdir(name, mode);
#elif defined(_WIN32)
  return mkdir(name);
#endif
}

void saveSettings(App* app) {
  char temp[256];

  sprintf(temp, "%sdata/config.cfg", app->basePath);

  FILE* file = fopen(temp, "w");
  if (file == NULL) {
    log_error("error while reading config.cfg\n");

    return;
  }

  fprintf(file, "volume:%d\n", app->settings.currentVolume);
  fprintf(file, "fullscreen:%d\n", (int32_t)app->settings.isFullscreen);
  fprintf(file, "allowedWeapons:4\n");
  for (int32_t i = 0; i < 4; ++i) {
    fprintf(file, "%d\n", (int32_t)app->settings.weaponsAllowed[i]);
  }

  fclose(file);
}

void readSettings(App* app) {
  char temp[256];

  sprintf(temp, "%sdata", app->basePath);
  if (createFolder(temp, 0755) == 0) {
    log_warn("data folder wasn't found and was created successfully! :)");
  }

  sprintf(temp, "%sdata/config.cfg", app->basePath);

  FILE* file = fopen(temp, "r");
  if (file == NULL) {
    log_warn("error while reading config.cfg, fixing it");

    saveSettings(app);
    return;
  }

  int32_t tempValue;

  if (fscanf(file, "volume:%d\n", &tempValue) < 1) {
    fclose(file);
    return;
  }
  if (tempValue >= 0 && tempValue <= 100) {
    app->settings.currentVolume = tempValue;
    Mix_Volume(-1, app->settings.currentVolume);
    Mix_VolumeMusic(app->settings.currentVolume);
  } else {
    app->settings.currentVolume = 0;
  }

  if (fscanf(file, "fullscreen:%d\n", &tempValue) < 1) {
    fclose(file);
    return;
  }
  if (tempValue == 0 || tempValue == 1) {
    app->settings.isFullscreen = tempValue;
    SDL_SetWindowFullscreen(app->window, app->settings.isFullscreen
                                             ? SDL_WINDOW_FULLSCREEN_DESKTOP
                                             : 0);
  } else {
    app->settings.isFullscreen = SDL_FALSE;
  }

  if (fscanf(file, "allowedWeapons:%d\n", &tempValue) < 1) {
    fclose(file);
    return;
  }
  for (int32_t i = 0; i < tempValue; ++i) {
    int32_t tempKey;
    if (fscanf(file, "%d\n", &tempKey) < 1) {
      fclose(file);
      return;
    }

    if (tempKey == 0 || tempKey == 1) {
      app->settings.weaponsAllowed[i] = (tempKey) ? SDL_TRUE : SDL_FALSE;
    } else {
      app->settings.weaponsAllowed[i] = SDL_FALSE;
    }
  }

  fclose(file);
  return;
}