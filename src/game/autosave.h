#ifndef AUTOSAVE_H
#define AUTOSAVE_H

#include "../App.h"

void clearSave(App* app);

void saveCurrentState(App* app, Player* firstPlayer, Player* secondPlayer,
                      int32_t* heightMap, int32_t isFirstTarget, uint32_t seed);

int32_t loadSavedState(App* app, Player* firstPlayer, Player* secondPlayer,
                       int32_t* heightMap, uint32_t* seed);

#endif