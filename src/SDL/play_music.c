#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stddef.h>
#include <stdio.h>

#include "log/log.h"

void playMusic() {
  // is music playing now
  if (Mix_PlayingMusic()) {
    log_warn("music is already playing");

    return;
  }

  // load music
  char musicPath[256];
  char* basePath = SDL_GetBasePath();
  if (basePath == NULL) {
    log_error("error getting base path: %s", SDL_GetError());

    return;
  }

  sprintf(musicPath, "%smedia/music/menu_theme.mp3", basePath);  // wav or mp3
  SDL_free(basePath);

  Mix_Music* backgroundMusic = Mix_LoadMUS(musicPath);
  if (backgroundMusic == NULL) {
    log_error("error loading music: %s", Mix_GetError());

    return;
  }
  log_info("music loaded successfully: %s", musicPath);

  // play music
  if (Mix_PlayMusic(backgroundMusic, -1) == -1) {  // -1 infinity loop
    log_error("error playing music: %s", Mix_GetError());

    Mix_FreeMusic(backgroundMusic);
    return;
  }
  log_info("music playback started");
}
