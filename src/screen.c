#include "ozone.h"

char valid_size(const _ENV *env) {
  return env->w==COLS && env->h==LINES;
}

void free_buffers(_ENV *env) {
  if (env->background)
    free(env->background);
  if (env->foreground)
    free(env->foreground);
}

void resize_buffers(_ENV *env) {
  if (valid_size(env))
    return;
  free_buffers(env);
  env->w = COLS;
  env->h = LINES;
  const long bufsz = sizeof(int)*COLS*LINES;
  env->background = malloc(bufsz);
  memset(env->background, 0, bufsz);
  env->foreground = malloc(bufsz);
  memset(env->foreground, 0, bufsz);
}
