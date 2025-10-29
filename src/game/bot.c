#include "bot.h"
#include "../UI/obstacle.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

#include "../math/math.h"
#include "../math/rand.h"
#include "log/log.h"
#include "player_movement.h"

void botMain(App* app, Player* player1, Player* player2, int32_t* heightMap,
             RenderObject* projectile, RenderObject* explosion,
             SDL_bool* regenMap, SDL_bool* recalcBulletPath,
             enum PlayerType playerType) {
  double initGunAngle = app->currPlayer->tankGunObj->data.texture.angle;

  // calculating angle specifically for a current player
  if (app->currPlayer == player2) {
    initGunAngle += 180 - app->currPlayer->tankGunObj->data.texture.angleAlt;
  } else {
    initGunAngle += app->currPlayer->tankGunObj->data.texture.angleAlt;
  }

  // normalizing just to be sure its in [0;2pi) and now its counterclockwise
  initGunAngle = 360 - normalizeAngle(initGunAngle);

  switch (playerType) {
    case EASY:
      while (calcBestOption(app, player1, player2, heightMap, projectile,
                            explosion, regenMap, recalcBulletPath, 55,
                            initGunAngle, playerType) &&
             app->currState == PLAY);
      break;
    case NORMAL:
      while (calcBestOption(app, player1, player2, heightMap, projectile,
                            explosion, regenMap, recalcBulletPath, 77,
                            initGunAngle, playerType) &&
             app->currState == PLAY);
      break;
    case HARD:
      while (calcBestOption(app, player1, player2, heightMap, projectile,
                            explosion, regenMap, recalcBulletPath, 90,
                            initGunAngle, playerType) &&
             app->currState == PLAY);
      break;
    default:
      log_info("[bot] default actiton for bot");
      break;
  }
}

// this func decide what option is the best
int32_t calcBestOption(App* app, Player* firstPlayer, Player* secondPlayer,
                       int32_t* heightMap, RenderObject* projectile,
                       RenderObject* explosion, SDL_bool* regenMap,
                       SDL_bool* recalcBulletPath, int32_t hitChance,
                       double initGunAngle, enum PlayerType playerType) {
  Player* enemy;
  if (app->currPlayer == firstPlayer) {
    enemy = secondPlayer;
  } else {
    enemy = firstPlayer;
  }

  // getting collison for an enemy player
  SDL_Point collisionP1, collisionP2, collisionP3;
  int32_t collisionP1R, collisionP2R, collisionP3R;
  // getting player collisions centers
  // left player collision
  if (app->currPlayer == secondPlayer) {
    collisionP1 = getPixelScreenPosition(
        (SDL_Point){enemy->tankObj->data.texture.scaleRect.x,
                    enemy->tankObj->data.texture.scaleRect.y},
        (SDL_Point){5 * app->scalingFactorX, 27 * app->scalingFactorY},
        enemy->tankObj->data.texture.angle,
        (SDL_Point){35 * app->scalingFactorX, 19 * app->scalingFactorY});
    collisionP2 = getPixelScreenPosition(
        (SDL_Point){enemy->tankObj->data.texture.scaleRect.x,
                    enemy->tankObj->data.texture.scaleRect.y},
        (SDL_Point){5 * app->scalingFactorX, 27 * app->scalingFactorY},
        enemy->tankObj->data.texture.angle,
        (SDL_Point){20 * app->scalingFactorX, 12 * app->scalingFactorY});
    collisionP3 = getPixelScreenPosition(
        (SDL_Point){enemy->tankObj->data.texture.scaleRect.x,
                    enemy->tankObj->data.texture.scaleRect.y},
        (SDL_Point){5 * app->scalingFactorX, 27 * app->scalingFactorY},
        enemy->tankObj->data.texture.angle,
        (SDL_Point){7 * app->scalingFactorX, 19 * app->scalingFactorY});
    // radiuses
    collisionP1R = 9 * MAX(app->scalingFactorX, app->scalingFactorY);
    collisionP2R = 13 * MAX(app->scalingFactorX, app->scalingFactorY);
    collisionP3R = 10 * MAX(app->scalingFactorX, app->scalingFactorY);
  }
  // right player collison
  else {
    collisionP1 = getPixelScreenPosition(
        (SDL_Point){enemy->tankObj->data.texture.scaleRect.x,
                    enemy->tankObj->data.texture.scaleRect.y},
        (SDL_Point){8 * app->scalingFactorX, 27 * app->scalingFactorY},
        enemy->tankObj->data.texture.angle,
        (SDL_Point){11 * app->scalingFactorX, 19 * app->scalingFactorY});
    collisionP2 = getPixelScreenPosition(
        (SDL_Point){enemy->tankObj->data.texture.scaleRect.x,
                    enemy->tankObj->data.texture.scaleRect.y},
        (SDL_Point){8 * app->scalingFactorX, 27 * app->scalingFactorY},
        enemy->tankObj->data.texture.angle,
        (SDL_Point){23 * app->scalingFactorX, 13 * app->scalingFactorY});
    collisionP3 = getPixelScreenPosition(
        (SDL_Point){enemy->tankObj->data.texture.scaleRect.x,
                    enemy->tankObj->data.texture.scaleRect.y},
        (SDL_Point){8 * app->scalingFactorX, 27 * app->scalingFactorY},
        enemy->tankObj->data.texture.angle,
        (SDL_Point){36 * app->scalingFactorX, 18 * app->scalingFactorY});
    // radiuses
    collisionP1R = 11 * MAX(app->scalingFactorX, app->scalingFactorY);
    collisionP2R = 11 * MAX(app->scalingFactorX, app->scalingFactorY);
    collisionP3R = 9 * MAX(app->scalingFactorX, app->scalingFactorY);
  }

  SDL_Point initPos = getPixelScreenPosition(
      (SDL_Point){app->currPlayer->tankObj->data.texture.scaleRect.x,
                  app->currPlayer->tankObj->data.texture.scaleRect.y},
      (SDL_Point){5 * app->scalingFactorX, 27 * app->scalingFactorY},
      app->currPlayer->tankObj->data.texture.angle,
      (SDL_Point){24 * app->scalingFactorX, 7 * app->scalingFactorY});

  initPos.x /= app->scalingFactorX;
  initPos.y /= app->scalingFactorY;

  initPos.x += 25 * cos(DEGTORAD(initGunAngle));
  initPos.y -= 25 * sin(DEGTORAD(initGunAngle));

  SDL_FPoint currPos = {
      .x = (float)initPos.x,
      .y = (float)initPos.y,
  };

  double velMultiplicator;
  int32_t explosionRadius;
  SDL_bool isHittableNearby;
  switch (app->currWeapon) {
    // small bullet
    case 0:
      velMultiplicator = 2;
      explosionRadius = projectile->data.texture.constRect.w;
      isHittableNearby = SDL_FALSE;
      break;
    // BIG BULLET
    case 1:
      velMultiplicator = 1.75;
      explosionRadius = projectile->data.texture.constRect.w;
      isHittableNearby = SDL_FALSE;
      break;
    // small boom
    case 2:
      velMultiplicator = 1.25;
      explosionRadius = projectile->data.texture.constRect.w * 2;
      isHittableNearby = SDL_TRUE;
      break;
    // BIG BOOM
    case 3:
      velMultiplicator = 1.0;
      explosionRadius = projectile->data.texture.constRect.w * 4;
      isHittableNearby = SDL_TRUE;
      break;
    default:
      velMultiplicator = 1.0;
      explosionRadius = projectile->data.texture.constRect.w;
      isHittableNearby = SDL_FALSE;
      break;
  }

  int32_t initVel = app->currPlayer->firingPower * velMultiplicator;
  int32_t rnnndmTest = getRandomValue(0, 100);
  SDL_bool isMissingShot = rnnndmTest > hitChance;
  if (isMissingShot) {
    log_info("[bot] MISSING CURRENT SHOT!!");
  }

  int32_t hitPos =
      calcHitPosition(&currPos, initVel, initGunAngle, heightMap, app,
                      &collisionP1, &collisionP2, &collisionP3, collisionP1R,
                      collisionP2R, collisionP3R, projectile);

  // if we hitted enemy in the tank (oh yeah, thats the best option)
  if (hitPos < -1) {
    log_info("[bot] bot found a move(collision hit) to shoot at %d", -hitPos);

    if (isMissingShot) {
      int32_t newAngle = app->currPlayer->gunAngle + getRandomValue(7, 10);
      int32_t newPower = app->currPlayer->firingPower + getRandomValue(7, 10);

      if (newAngle < 0) {
        newAngle = 0;
      } else if (newAngle > 120) {
        newAngle = 120;
      }
      if (newPower < 1) {
        newPower = 1;
      }
      if (newPower > 100) {
        newPower = 100;
      }

      smoothChangeAngle(app->currPlayer, newAngle, &app->currState,
                        recalcBulletPath);
      smoothChangePower(app->currPlayer, newPower, &app->currState,
                        recalcBulletPath);
    }

    SDL_Delay(200);

    shoot(app, firstPlayer, secondPlayer, projectile, explosion, heightMap,
          regenMap);
    recalcPlayerPos(app, firstPlayer, heightMap, 0, 5);
    recalcPlayerPos(app, secondPlayer, heightMap, 0, 8);
    return 0;
  }

  // hit will be counted if its in [leftCorner; rightCorner] with a really small error (its nearly to zero)
  int32_t leftCorner =
      enemy->tankObj->data.texture.constRect.x * app->scalingFactorX -
      explosionRadius;
  int32_t rightCorner =
      enemy->tankObj->data.texture.constRect.x * app->scalingFactorX +
      enemy->tankObj->data.texture.constRect.w * app->scalingFactorX *
          cos(DEGTORAD(enemy->tankAngle)) +
      explosionRadius;

  if (hitPos >= leftCorner && hitPos <= rightCorner && isHittableNearby) {
    log_info("[bot] bot found a move(nearby hit) to shoot at %d", hitPos);

    if (playerType == NORMAL || playerType == EASY) {
      if (isMissingShot) {
        int32_t newAngle = app->currPlayer->gunAngle + getRandomValue(4, 7);
        int32_t newPower = app->currPlayer->firingPower + getRandomValue(4, 7);

        if (newAngle < 0) {
          newAngle = 0;
        } else if (newAngle > 120) {
          newAngle = 120;
        }
        if (newPower < 1) {
          newPower = 1;
        }
        if (newPower > 100) {
          newPower = 100;
        }

        smoothChangeAngle(app->currPlayer, newAngle, &app->currState,
                          recalcBulletPath);
        smoothChangePower(app->currPlayer, newPower, &app->currState,
                          recalcBulletPath);
      }
      log_info("[bot] SHOOT!");

      shoot(app, firstPlayer, secondPlayer, projectile, explosion, heightMap,
            regenMap);
      recalcPlayerPos(app, firstPlayer, heightMap, 0, 5);
      recalcPlayerPos(app, secondPlayer, heightMap, 0, 8);
      return 0;
    } else {
      log_info("[bot] trying to find a better move!");
    }
  }

  log_info("[bot] bot found only a poopy doopy move to shoot at %d", hitPos);
  log_info("[bot] trying to find better move");

  SDL_bool isFinded = SDL_FALSE;

  for (int32_t angle = 0; angle <= 120; ++angle) {
    double currAngle = app->currPlayer->tankGunObj->data.texture.angle;

    // calculating angle specifically for a current player
    if (app->currPlayer == secondPlayer) {
      currAngle += 180 + angle;
    } else {
      currAngle += -angle;
    }

    currAngle = round(currAngle);

    // normalizing just to be sure its in [0;2pi) and now its counterclockwise
    currAngle = 360 - normalizeAngle(currAngle);

    for (int32_t firingPower = 0; firingPower <= 99; ++firingPower) {
      int32_t hitPos = calcHitPosition(&currPos, firingPower * velMultiplicator,
                                       currAngle, heightMap, app, &collisionP1,
                                       &collisionP2, &collisionP3, collisionP1R,
                                       collisionP2R, collisionP3R, projectile);
      // collision hit
      if (hitPos < -1) {
        if (!isFinded) {
          isFinded = SDL_TRUE;
          angle += 5;
          break;
        }

        log_info("[bot] bot found the best move for FP:%d and angle:%f",
                 firingPower, currAngle);

        if (isMissingShot) {
          angle += getRandomValue(2, 5);
          firingPower += getRandomValue(2, 5);

          if (angle < 0) {
            angle = 0;
          } else if (angle > 120) {
            angle = 120;
          }
          if (firingPower < 1) {
            firingPower = 1;
          } else if (firingPower > 100) {
            firingPower = 100;
          }
        }

        smoothChangeAngle(app->currPlayer, angle, &app->currState,
                          recalcBulletPath);
        smoothChangePower(app->currPlayer, firingPower, &app->currState,
                          recalcBulletPath);
        SDL_Delay(200);
        shoot(app, firstPlayer, secondPlayer, projectile, explosion, heightMap,
              regenMap);
        recalcPlayerPos(app, firstPlayer, heightMap, 0, 5);
        recalcPlayerPos(app, secondPlayer, heightMap, 0, 8);
        return 0;
      }
    }
  }

  if (app->currPlayer->movesLeft == 0) {
    shoot(app, firstPlayer, secondPlayer, projectile, explosion, heightMap,
          regenMap);
    return 0;
  } else {
    if (app->currPlayer == firstPlayer) {
      smoothMove(app, SDL_TRUE, SDL_TRUE, heightMap, obstacleRock);
      return 1;
    } else {
      smoothMove(app, SDL_FALSE, SDL_FALSE, heightMap, obstacleRock);
      return 1;
    }
  }
  return 1;
}