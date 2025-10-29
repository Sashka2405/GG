#ifndef PLAY_RENDER_H
#define PLAY_RENDER_H

#include "../App.h"

void playMain(App* app, uint32_t SEED);
void preGameMain(App* app);
void renderMap(SDL_Renderer* renderer, int32_t* heightmap, int32_t* basedMap,
               int32_t width, int32_t height);
SDL_Texture* saveRenderMapToTexture(SDL_Renderer* renderer, int32_t width,
                                    int32_t height, int32_t* heightMap,
                                    int32_t* basedMap);

#endif