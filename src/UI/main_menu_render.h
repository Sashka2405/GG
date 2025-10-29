#ifndef LOGO_RENDER_H
#define LOGO_RENDER_H

#include "../App.h"

void mainMenu(App* app);
void SDL_Cleanup(App* app);
void menuRenderLoop(App* app, RenderObject* logo);

#endif