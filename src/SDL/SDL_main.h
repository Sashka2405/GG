#ifndef SDL_MAIN_H
#define SDL_MAIN_H

#include "../App.h"

int32_t SDL_Initialise(App*);
void SDL_Cleanup(App*);
void SDL_Main();
void mainGameLoop(App* app);

void loadAllSounds(App* app);
void startMusic(const App* app);
void createWindow(App* app);
void initCursor(App* app);

void mainMenu(App*);

#endif