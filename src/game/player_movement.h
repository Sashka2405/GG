#ifndef PLAYERMOVEMENT_H
#define PLAYERMOVEMENT_H

#include "../App.h"

void shoot(App* app, Player* firstPlayer, Player* secondPlayer,
           RenderObject* projectile, RenderObject* explosion,
           int32_t* heightMap, SDL_bool* regenMap);
void recalcPlayerPos(App* app, Player* player, int32_t* heightMap, int32_t dx,
                     int32_t xOffset);
void recalcPlayerGunAngle(Player* player, int32_t dy);
int32_t playerMove(void* data);

#endif