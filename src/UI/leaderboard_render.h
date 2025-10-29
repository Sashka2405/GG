#ifndef LEADERBOARD_BOARD
#define LEADERBOARD_BOARD

#include "../App.h"

typedef struct PlayerScore {
  int32_t score;
  char name[16];
} PlayerScore;

void leaderboardAddMain(App* app);
void addToLeaderBoard(const char* name, int32_t score);
void leaderboardMain(App* app, const char* name);

#endif