#ifndef BOT_H
#define BOT_H

#include "../App.h"

;
void botMain(App* app, Player* player1, Player* player2, int32_t* heightMap,
             RenderObject* projectile, RenderObject* explosion,
             SDL_bool* regenMap, SDL_bool* recalcBulletPath,
             enum PlayerType playerType);

int32_t calcBestOption(App* app, Player* firstPlayer, Player* secondPlayer,
                       int32_t* heightMap, RenderObject* projectile,
                       RenderObject* explosion, SDL_bool* regenMap,
                       SDL_bool* recalcBulletPath, int32_t hitChance,
                       double initGunAngle, enum PlayerType playerType);

#endif