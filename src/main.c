#include "log/log.h"

void SDL_Main();
void checkOS();

int main() {
#ifdef DEBUG
  // output all logs to stderr
  log_set_level(0);
#else
  // output only error and fatal if release
  log_set_level(4);
#endif
  checkOS();
  SDL_Main();
  return 0;
}

void checkOS() {
#if defined(__unix__)
  log_debug("Detected unix-like OS");
#elif defined(_WIN32)
  log_debug("Detected Woondows =(");
#else
  log_fatal("NON SUPPORTED OS!!!");
  exit(-1337);
#endif
}