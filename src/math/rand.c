#include "rand.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <wincrypt.h>
#endif

#include "log/log.h"

// func return a random value from a native linux file
// or using windiws crypt lib
int32_t getRandomValue(int32_t min, int32_t max) {
  int32_t res;
#if defined(__unix__)
  res = getRandomDWORD_unix();
#elif defined(_WIN32)
  res = getRandomDWORD_win();
#endif
  res = (res % (max - min + 1));
  if (res < 0) {
    res += max - min + 1;
  }
  return res + min;
}

#if defined(__unix__)
int32_t getRandomDWORD_unix() {
  int32_t res;
  FILE* randFile = fopen("/dev/urandom", "r");
  if (randFile == NULL) {
    log_fatal("Error opening /dev/urandom file");
    exit(-1);
  }
  if (!fread(&res, sizeof(res), 1, randFile)) {
    fclose(randFile);
    log_fatal("Error while reading rand value");
    exit(-1);
  }
  fclose(randFile);

  return res;
}
#endif

#if defined(_WIN32)
int32_t getRandomDWORD_win() {
  HCRYPTPROV hProv;
  uint32_t randomValue = 0;

  if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL,
                           CRYPT_VERIFYCONTEXT)) {
    log_fatal("Can't get crypt context");
    exit(-1);
  }

  if (!CryptGenRandom(hProv, sizeof(randomValue), (BYTE*)&randomValue)) {
    log_fatal("Can't generate random bytes");
    CryptReleaseContext(hProv, 0);
    exit(-1);
  }
  CryptReleaseContext(hProv, 0);
  return randomValue;
}
#endif