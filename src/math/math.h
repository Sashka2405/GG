#ifndef BASIC_MATH_H
#define BASIC_MATH_H

#include "../App.h"

#define DEGTORAD(x) (x * M_PI / 180.0)
#define RADTODEG(x) (x * 180.0 / M_PI)
#define MAX(x, y) (x > y ? x : y)

double getAngle(int32_t x, int32_t* heightMap, int32_t dx);
SDL_Point getPixelScreenPosition(SDL_Point drawPos, SDL_Point center,
                                 double angleDeg, SDL_Point target);
double normalizeAngle(double angleDeg);

int32_t getAllowedNumber(App* app);

void getPositionAtSpecTime(SDL_FPoint* pos, double vx, double vy, double windVx,
                           double windVy, double currTime);

int32_t calcHitPosition(SDL_FPoint* initPos, double initVel, double angle,
                        int32_t* heightMap, const App* app,
                        const SDL_Point* collision1, const SDL_Point* collison2,
                        const SDL_Point* collision3, const int32_t collision1R,
                        const int32_t collision2R, const int32_t collision3R,
                        RenderObject* projectile);

void smoothChangeAngle(Player* player, int32_t endAngle, enum State* currState,
                       SDL_bool* recalcBulletPath);
void smoothChangePower(Player* player, int32_t endPower, enum State* currState,
                       SDL_bool* recalcBulletPath);

int32_t smoothMove(App* app, SDL_bool isFirstPlayer, SDL_bool isRight,
                   int32_t* heightMap, uint32_t* obstacle);

SDL_bool isInTriangle(const int32_t x, const int32_t y, SDL_Point p1,
                      SDL_Point p2, SDL_Point p3);
SDL_bool isInCircle(const int32_t x, const int32_t y, const SDL_Point* center,
                    const int32_t radius);

#endif