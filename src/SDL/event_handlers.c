#include "event_handlers.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include "SDL_render.h"
#include "log/log.h"

// function for thread that replace cursor32.png with cursor32Triggered.png
int32_t threadCursorTrigger(void* arg) {
  App* app = arg;
  while (app->currState != EXIT) {
    if (app->isMouseTriggered) {
      SDL_SetCursor(app->cursorTriggered);
    } else {
      SDL_SetCursor(app->cursor);
    }
    SDL_Delay(16);
  }
  return 0;
}

// function for thread that handle different events every 16ms (60 fps)
int32_t threadEventPoll(void* arg) {
  App* app = arg;
  SDL_Event currEvent;
  while (SDL_PollEvent(&currEvent)) {
    switch (currEvent.type) {
      // is it closing the app?
      case SDL_QUIT: {
        log_info(
            "thread success detected SDL_QUIT like "
            "event. Setting STATUS TO EXIT");
        app->currState = EXIT;
        break;
      }
      // has window been resized?
      case SDL_WINDOWEVENT: {
        if (currEvent.window.event == SDL_WINDOWEVENT_RESIZED) {
          log_info(
              "hread success detected RESIZE like "
              "event. Fixing textures sizes");

          SDL_GetWindowSize(app->window, &app->screenWidth, &app->screenHeight);
          app->scalingFactorX = app->screenWidth / 1024.0;
          app->scalingFactorY = app->screenHeight / 768.0;
          log_info("curr scaling X:%lf (%d), Y:%lf (%d)", app->scalingFactorX,
                   app->screenWidth, app->scalingFactorY, app->screenHeight);
        }
        break;
      }
      // is some key down?
      case SDL_KEYDOWN: {
        keyPressHandler(app, currEvent);
        break;
      }
      // is LMB pressed?
      case SDL_MOUSEBUTTONDOWN: {
        if (currEvent.button.button == SDL_BUTTON_LEFT) {
          app->isMouseDragging = SDL_TRUE;
        }
        break;
      }

      // is LMB released?
      case SDL_MOUSEBUTTONUP: {
        if (currEvent.button.button == SDL_BUTTON_LEFT) {
          app->isMouseDragging = SDL_FALSE;
          int32_t x, y;
          SDL_GetMouseState(&x, &y);
          log_info(
              "detected mouse click (lbm_up) in (%d, "
              "%d)",
              x, y);

          LMBReleaseHandle(app);
        }
        break;
      }
    }
  }
  return 0;
}

void proceedSlider(App* app, int32_t sliderPosX) {
  switch (app->buttonPosTriggered) {
    case s_VOLUME:
      app->settings.currentVolume = sliderPosX;
      break;
    default:
      break;
  }
}

void proceedShiftedButtons(App* app, enum Button buttonName) {
  switch (buttonName) {
    case bT_DECREMENT_VOLUME:
      log_info(
          "thread success detected \"volume -\" was "
          "pressed. Decresing volume by 1");

      app->settings.currentVolume = app->settings.currentVolume - 1 < 0
                                        ? 0
                                        : app->settings.currentVolume - 1;
      break;
    case bT_INCREMENT_VOLUME:
      log_info(
          "thread success detected \"volume +\" was "
          "pressed. Increasing volume by 1");

      app->settings.currentVolume = app->settings.currentVolume + 1 > 100
                                        ? 100
                                        : app->settings.currentVolume + 1;
      break;
    default:
      break;
  }
}

// proceed lmb clicks
void LMBReleaseHandle(App* app) {
  SDL_bool wasTriggeredObjTriggered = SDL_FALSE;
  switch (app->buttonPosTriggered) {
    case b_PLAY:
      log_info(
          "thread success detected \"PLAY\" was "
          "pressed.");

      app->currState = PREGAME_SETTING;
      break;
    case b_SETTINGS:
      log_info(
          "thread success detected \"SETTINGS\" was "
          "pressed.");

      app->currState = SETTINGS;
      break;
    case b_QUIT:
      log_info(
          "thread success detected \"QUIT\" was "
          "pressed. quiting");

      if (app->currState == PLAY) {
        if (app->currPlayer != NULL && !app->currPlayer->inAnimation &&
            app->currPlayer->type == MONKE) {
          app->currState = EXIT;
        }
      } else {
        app->currState = EXIT;
      }
      break;
    case b_MINIMIZE:
      log_info(
          "thread success detected minimize button "
          "was pressed. Minimizing");

      SDL_MinimizeWindow(app->window);
      break;
    case b_LEADERBOARD_CONTINUE:
      log_info("thread success detected leaderboard_continue button.");

      app->currState = LEADERBOARD;
      break;
    case b_SETTINGS_BACK:
      log_info(
          "thread success detected \"back\" was "
          "pressed");

      app->currState = LOGO;
      break;
    case b_CHECKBOX_FULLSCREEN:
      log_info(
          "thread success detected \"fullscreen "
          "checkbox\" was pressed. Toggling checkbox state");

      // switch checkbox
      app->settings.isFullscreen = !app->settings.isFullscreen;
      SDL_SetWindowFullscreen(app->window, app->settings.isFullscreen
                                               ? SDL_WINDOW_FULLSCREEN_DESKTOP
                                               : 0);
      break;
    case b_PREGAME_START:
      if (app->p1Diff != b_NULL && app->p2Diff != b_NULL) {
        app->currState = PLAY;
        log_info(
            "thread success detected \"START\" was "
            "pressed. Setting state to PLAY");
      }
      break;
    case b_PREGAME_LOAD:
      log_info(
          "thread success detected \"LOAD\" was "
          "pressed. Setting state to LOAD");

      app->currState = LOAD;
      break;
    case b_HELP:
      log_info(
          "thread success detected \"HELP\" was "
          "pressed. Setting state to HELP");

      app->currState = HELP;
      break;
    case b_LEADERBOARDS:
      log_info(
          "thread success detected \"LEADERBOARDS\" "
          "was pressed. Setting state to LEADERBOARD");

      app->currState = LEADERBOARD;
      break;
    case bTI_SEED:
      log_info("thread success detected \"SEED\" triggered");

      app->currentTriggeredObject = bTI_SEED;
      wasTriggeredObjTriggered = SDL_TRUE;
      break;
    case bTI_NAME:
      log_info("thread success detected \"NAME\" triggered");

      app->currentTriggeredObject = bTI_NAME;
      wasTriggeredObjTriggered = SDL_TRUE;
      break;
    case b_P1Player:
    case b_P1Easy:
    case b_P1Normal:
    case b_P1Hard:
      app->p1Diff = app->buttonPosTriggered;
      break;
    case b_P2Player:
    case b_P2Easy:
    case b_P2Normal:
    case b_P2Hard:
      app->p2Diff = app->buttonPosTriggered;
      break;
    case b_W1:
      if (app->settings.weaponsAllowed[0] + app->settings.weaponsAllowed[1] +
              app->settings.weaponsAllowed[2] +
              app->settings.weaponsAllowed[3] >
          2) {
        app->settings.weaponsAllowed[0] = !app->settings.weaponsAllowed[0];
      } else {
        app->settings.weaponsAllowed[0] = SDL_TRUE;
      }
      break;
    case b_W2:
      if (app->settings.weaponsAllowed[0] + app->settings.weaponsAllowed[1] +
              app->settings.weaponsAllowed[2] +
              app->settings.weaponsAllowed[3] >
          2) {
        app->settings.weaponsAllowed[1] = !app->settings.weaponsAllowed[1];
      } else {
        app->settings.weaponsAllowed[1] = SDL_TRUE;
      }
      break;
    case b_W3:
      if (app->settings.weaponsAllowed[0] + app->settings.weaponsAllowed[1] +
              app->settings.weaponsAllowed[2] +
              app->settings.weaponsAllowed[3] >
          2) {
        app->settings.weaponsAllowed[2] = !app->settings.weaponsAllowed[2];
      } else {
        app->settings.weaponsAllowed[2] = SDL_TRUE;
      }
      break;
    case b_W4:
      if (app->settings.weaponsAllowed[0] + app->settings.weaponsAllowed[1] +
              app->settings.weaponsAllowed[2] +
              app->settings.weaponsAllowed[3] >
          2) {
        app->settings.weaponsAllowed[3] = !app->settings.weaponsAllowed[3];
      } else {
        app->settings.weaponsAllowed[3] = SDL_TRUE;
      }
      break;
    default:
      break;
  }
  if (!wasTriggeredObjTriggered) {
    app->currentTriggeredObject = b_NONE;
  }
}

// proceed key presses
void keyPressHandler(App* app, SDL_Event currEvent) {
  switch (currEvent.key.keysym.sym) {
    // if ESC was pressed
    case SDLK_ESCAPE: {
      log_info(
          "thread success detected \"ESC\" button was "
          "pressed. trying to set status -=1");

      // going back in menu if "esc" is pressed
      // but if player is not in game
      switch (app->currState) {
        case SETTINGS:
        case PREGAME_SETTING:
        case HELP:
          app->currState = LOGO;
          break;
        case MENU:
        case LOGO:
          app->currState = EXIT;
          break;
        case PLAY:
          app->currState = LOGO;
          break;
        case LEADERBOARD_ADD:
          app->currState = LOGO;
          break;
        case LEADERBOARD:
          app->currState = LOGO;
          break;
        default:
          break;
      }
      break;
    }
    default: {
      // if triggered object was pressed
      if (app->currentTriggeredObject >= 1500 &&
          app->currentTriggeredObject <= 1999)
        app->curPressedKey = currEvent.key.keysym.sym;
      break;
    }
  }
}

char filterPressedKey(SDL_Keycode key, int32_t flags) {
  // latin letters
  if ((flags & 1) && SDLK_a <= key && SDLK_z >= key) {
    return key;
  }

  // nums letters
  if ((flags & 2) && SDLK_0 <= key && SDLK_9 >= key) {
    return key;
  }

  // backspace button
  if (key == SDLK_BACKSPACE) {
    return key;
  }

  return -1;
}

void proceedTextInputLine(App* app, RenderObject* currObj) {
  if (currObj->buttonName == app->currentTriggeredObject) {
    char currentPressedKey = filterPressedKey(
        app->curPressedKey, currObj->data.textInputLine.charTypes);
    app->curPressedKey = 0;
    // if we cant enter this char
    if (currentPressedKey == -1) {
      return;
    }

    int32_t textLen = strlen(currObj->data.textInputLine.savedText);
    // removing last elemets
    if (textLen >= 0 && currentPressedKey == SDLK_BACKSPACE) {
      if (textLen == 0) return;

      currObj->data.textInputLine.savedText[textLen - 1] = '\0';
      // updating texture
      SDL_DestroyTexture(currObj->data.textInputLine.textTexture);
      // if str after that is not empty
      if (textLen > 1) {
        currObj->data.textInputLine.textTexture = createTextTexture(
            app->renderer, currObj->data.textInputLine.font,
            currObj->data.textInputLine.savedText, 255, 255, 255, 255);
      }
      return;
    }

    // if we're above the limit
    if (textLen >= 31 || textLen >= currObj->data.textInputLine.maxInputChars) {
      return;
    }

    currObj->data.textInputLine.savedText[textLen] = currentPressedKey;
    //updating texture
    SDL_DestroyTexture(currObj->data.textInputLine.textTexture);
    currObj->data.textInputLine.textTexture = createTextTexture(
        app->renderer, currObj->data.textInputLine.font,
        currObj->data.textInputLine.savedText, 255, 255, 255, 255);
  }
}