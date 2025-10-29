#ifndef APP_H
#define APP_H

#include "game/specialConditions/windStruct.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

enum State {
  EXIT = -1,
  LOGO,
  MENU,
  HELP,
  SETTINGS,
  PREGAME_SETTING,
  PLAY,
  LOAD,
  LEADERBOARD_ADD,
  LEADERBOARD,
};

enum PlayerType {
  // real player
  MONKE = 0,
  // bots diffs
  EASY,
  NORMAL,
  HARD,
};

enum Button {
  b_NULL = -1,
  // buttons
  b_NONE = 0,
  b_PLAY,
  b_SETTINGS,
  b_QUIT,
  b_MINIMIZE,
  b_SETTINGS_BACK,
  b_GRAPHICS_SETTINGS,
  b_WEAPON_SELECTION,
  b_CHECKBOX_FULLSCREEN,
  b_PREGAME_START,
  b_PREGAME_LOAD,
  b_ABOUT,
  b_HELP,
  b_LEADERBOARDS,
  b_LEADERBOARD_CONTINUE,
  // triangle buttons
  bT_INCREMENT_VOLUME = 500,
  bT_DECREMENT_VOLUME,
  // sliders
  s_VOLUME = 1000,
  // text input
  bTI_SEED = 1500,
  bTI_NAME = 1501,
  // tank guns
  LEFT_GUN = 2000,
  RIGHT_GUN = 2001,
  // difficulty choice button
  b_P1Player = 2100,
  b_P1Easy,
  b_P1Normal,
  b_P1Hard,
  b_P2Player,
  b_P2Easy,
  b_P2Normal,
  b_P2Hard,
  // weapons choice button
  b_W1 = 2200,
  b_W2,
  b_W3,
  b_W4,
};

typedef struct SettingsStruct {
  SDL_bool isFullscreen;
  int32_t currentVolume;
  SDL_bool weaponsAllowed[4];
} settingsStruct;

typedef struct RenderObject {
  union {
    struct {
      SDL_Texture* texture;
      SDL_Texture* triggeredTexture;
      SDL_Rect constRect;
      SDL_Rect scaleRect;
      double angle;
      double angleAlt;
      SDL_RendererFlip flipFlag;
      SDL_Point* centerRot;
      SDL_Point* centerRot_Alt;
      uint32_t currFrame;
      uint32_t maxFrames;
      int32_t fixedHeight;
      int32_t fixedWidth;
      int32_t framesWaitCoeff;
      SDL_bool singleShot;
    } texture;
    struct {
      // const points
      SDL_Point p1C, p2C, p3C;
      // points for scaling
      SDL_Point p1S, p2S, p3S;
    } triangle;
    struct {
      SDL_Rect constRect;
      SDL_Rect scaleRect;
      int32_t maxInputChars;
      enum {
        LAITN = 1 << 0,
        NUMS = 1 << 1,
        SPECIAL = 1 << 2,
      } charTypes;
      char savedText[32];
      TTF_Font* font;
      SDL_Texture* textTexture;
    } textInputLine;
  } data;
  SDL_Color normalColor;
  SDL_Color triggeredColor;
  int32_t Zpos;
  int32_t canBeTriggered;
  enum Button buttonName;
  SDL_bool disableRendering;
} RenderObject;

typedef struct Weapon {
  RenderObject* weaponObj;
  int32_t mass;
  int32_t explosivePower;
  enum WeaponType {
    SMALL,
    BIG,
    SMALL_BOOM,
    BIG_BOOM,
  } type;
} Weapon;

typedef struct Player {
  // tank vars
  RenderObject* tankObj;
  double tankAngle;
  int32_t x, y;

  // gun vars
  int32_t firingPower;
  double gunAngle;
  RenderObject* tankGunObj;
  int32_t score;

  int32_t movesLeft;

  // flag will be set if this object is in animation
  SDL_bool inAnimation;

  enum PlayerType type;
} Player;

typedef struct SpecialGlobalConditions {
  Wind wind;
} SpecialGlobalConditions;

typedef struct App {
  // main sdl stuff
  SDL_Window* window;
  SDL_Renderer* renderer;

  // for special loaded cursor
  SDL_Cursor* cursor;
  SDL_Cursor* cursorTriggered;

  // pwd path saved
  char* basePath;

  // for special loaded cursor
  SDL_Thread* cursorTriggerThread;

  // saving keystate arr provided by sdl
  const Uint8* keyStateArr;
  // current pressed key on keyboard
  SDL_KeyCode curPressedKey;

  enum State currState;

  // screen settings
  int32_t screenWidth;
  int32_t screenHeight;
  double scalingFactorX;
  double scalingFactorY;

  // for mouse
  int32_t xMouse;
  int32_t yMouse;
  int32_t isMouseDragging;
  int32_t isMouseTriggered;

  // for triggered buttons
  enum Button currentTriggeredObject;
  enum Button buttonPosTriggered;

  settingsStruct settings;

  Player* currPlayer;

  Mix_Chunk* sounds[2];

  // difficulty choice buttons
  enum Button p1Diff;
  enum Button p2Diff;

  // current player weapon
  int32_t currWeapon;

  // in game results this would
  // store winner and winner score
  int32_t winnerScore;
  int32_t winner;

  // how many shots were fired
  int32_t timesPlayed;

  // storing global special conditions here!
  SpecialGlobalConditions globalConditions;
} App;

#endif