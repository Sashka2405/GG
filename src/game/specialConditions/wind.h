#ifndef WIND_H
#define WIND_H

#include "../../App.h"

void updateWind(App* app);

void updateWindDirectionIcon(RenderObject* windDirectionIcon,
                             enum WindDirection direction);
void updateWindSpeedLabel(App* app, RenderObject* speedLabel,
                          double windStrength);

#endif