#ifndef WIND_STRUCT_H
#define WIND_STRUCT_H

// struct describing possible wind directions
// excluding S and N cuz it would be strange
// when bullet flies straight down or up
enum WindDirection {
  E = 0,  // 0
  SE,     // pi/4
  SW,     // 3pi/4
  W,      // pi
  NW,     // 5pi/4
  NE,     // 7pi/4
};

typedef struct {
  double windStrength;
  enum WindDirection windDirection;

  struct RenderObject* directionIcon;
  struct RenderObject* speedLabel;
} Wind;

#endif