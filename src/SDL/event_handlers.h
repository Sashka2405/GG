#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "../App.h"

int32_t threadEventPoll(void* arg);
int32_t threadCursorTrigger(void* arg);

char filterPressedKey(SDL_Keycode key, int32_t flags);

void LMBReleaseHandle(App* app);
void keyPressHandler(App* app, SDL_Event currEvent);
void proceedSlider(App* app, int32_t sliderPosX);
void proceedShiftedButtons(App* app, enum Button buttonName);
void proceedTextInputLine(App* app, RenderObject* currObj);

#endif