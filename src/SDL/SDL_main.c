#include "SDL_main.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include "../UI/help_render.h"
#include "../UI/leaderboard_render.h"
#include "../UI/play_render.h"
#include "../UI/settings_render.h"
#include "../game/settings.h"
#include "event_handlers.h"
#include "log/log.h"
#include "play_music.h"

void SDL_Main() {
  App mainGameWindow = {
      .screenWidth = 1024,
      .screenHeight = 768,
      .currState = LOGO,
      .scalingFactorX = 1.0,
      .scalingFactorY = 1.0,
      .isMouseTriggered = SDL_FALSE,
      .buttonPosTriggered = b_NONE,
      .settings =
          (settingsStruct){
              .isFullscreen = SDL_FALSE,
              .currentVolume = 10,
              .weaponsAllowed = {SDL_TRUE, SDL_TRUE, SDL_TRUE, SDL_TRUE},
          },
      .p1Diff = b_NULL,
      .p2Diff = b_NULL,
      .basePath = SDL_GetBasePath(),
  };
  // initialising main window
  if (SDL_Initialise(&mainGameWindow)) {
    // if it fails
    SDL_Cleanup(&mainGameWindow);
  }

  createWindow(&mainGameWindow);

  // read settings from file
  readSettings(&mainGameWindow);

  // load all nec. sounds
  loadAllSounds(&mainGameWindow);

  // load music, sounds and play it or store
  startMusic(&mainGameWindow);

  // loading cursor
  //initCursor(&mainGameWindow);

  // creating thread that will loop while SDL_QUIT event is not polled
  //mainGameWindow.eventPollThread =
  //SDL_CreateThread(threadEventPoll, "eventPoll", &mainGameWindow);
  // changing cursor when triggered
  mainGameWindow.cursorTriggerThread =
      SDL_CreateThread(threadCursorTrigger, "cursorTrigger", &mainGameWindow);

  // state picker
  mainGameLoop(&mainGameWindow);
  // exit
  SDL_Cleanup(&mainGameWindow);
}

// initialising all necessarily
int32_t SDL_Initialise(App* app) {
  // initialising all subsystems
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
    // if it fails
    log_fatal("error while initialising SDL: %s", SDL_GetError());
    return 1;
  }

  // initialising font loader
  if (TTF_Init()) {
    // if it fails
    log_fatal("error while initialising TTF: %s", TTF_GetError());
    return 2;
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    log_fatal("error while initialising MIXER: %s", Mix_GetError());
    return 6;
  }

  // initialising 'png' loader
  int32_t imgInitFlags = IMG_Init(IMG_INIT_PNG);
  if ((imgInitFlags & IMG_INIT_PNG) != IMG_INIT_PNG) {
    // if failed
    log_fatal("error while initialising IMG: %s", SDL_GetError());
    return 3;
  }
  // getting keyStateArr
  app->keyStateArr = SDL_GetKeyboardState(NULL);
  return 0;
}

void loadAllSounds(App* app) {
  char temp[256];

  sprintf(temp, "%smedia/music/gunshot.wav", app->basePath);

  app->sounds[0] = Mix_LoadWAV(temp);
  if (!app->sounds[0]) {
    log_error("error: %s", Mix_GetError());
  }
  log_info("loaded %s", temp);

  sprintf(temp, "%smedia/music/win.wav", app->basePath);

  app->sounds[1] = Mix_LoadWAV(temp);
  if (!app->sounds[1]) {
    log_error("error: %s", Mix_GetError());
  }
  log_info("loaded %s", temp);
}

// clean up SDL, img and other sh.t
void SDL_Cleanup(App* app) {
  SDL_WaitThread(app->cursorTriggerThread, NULL);

  SDL_FreeCursor(app->cursor);

  SDL_DestroyRenderer(app->renderer);
  SDL_DestroyWindow(app->window);

  Mix_FreeChunk(app->sounds[0]);
  Mix_FreeChunk(app->sounds[1]);
  Mix_CloseAudio();

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  free(app->basePath);

  log_info("SUCCESS CLEANUP!");
  exit(0);
}

void mainGameLoop(App* app) {
  // main loop
  while (app->currState != EXIT) {
    switch (app->currState) {
      case LOGO:
        log_info("current state: LOGO");
        mainMenu(app);
        break;
      case SETTINGS:
        log_info("current state: SETTINGS");
        settingsMain(app);
        break;
      case PLAY:
      case PREGAME_SETTING:
        log_info("current state: PREGAME_SETTING");
        preGameMain(app);
        break;
      case HELP:
        log_info("current state: HELP");
        helpMain(app);
        break;
      case LEADERBOARD_ADD:
        log_info("current state: LEADERBOARD_ADD");
        leaderboardAddMain(app);
        break;
      case LEADERBOARD:
        log_info("current state: LEADERBOARD");
        leaderboardMain(app, "");
        break;
      default:
        break;
    }
    SDL_Delay(16);
  }
}

void startMusic(const App* app) {
  playMusic();
  Mix_VolumeMusic(app->settings.currentVolume);
  Mix_Volume(-1, app->settings.currentVolume);
}

void initCursor(App* app) {
  char temp[256];
  sprintf(temp, "%smedia/imgs/cursor32.png", app->basePath);
  // loading cursor
  SDL_Surface* cursorSurface = IMG_Load(temp);
  app->cursor = SDL_CreateColorCursor(cursorSurface, 0, 0);
  SDL_SetCursor(app->cursor);

  sprintf(temp, "%smedia/imgs/cursor32Triggered.png", app->basePath);
  // loading triggered cursor
  SDL_Surface* cursorTriggeredSurface = IMG_Load(temp);
  app->cursorTriggered = SDL_CreateColorCursor(cursorTriggeredSurface, 0, 0);

  // freeing cursor surface
  SDL_FreeSurface(cursorSurface);
  SDL_FreeSurface(cursorTriggeredSurface);
}

void createWindow(App* app) {
  app->window = SDL_CreateWindow("Jonkler Tanks: Betment edition",
                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 app->screenWidth, app->screenHeight, 0);
  if (!app->window) {
    // if failed
    log_fatal("error while creating Window: %s", SDL_GetError());
  }

  // creating renderer
  app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);
  if (!app->renderer) {
    // if it fails
    log_fatal("error while creating Renderer: %s", SDL_GetError());
  }

  // enabling "linear" smoothing
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
}