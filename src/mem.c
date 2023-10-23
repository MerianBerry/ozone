#include "ozone.h"

void *mem_todynamic(void *mem, size_t size) {
  void *p = malloc(size);
  if (!p)
    return NULL;
  memset(p, 0, size);
  memcpy(p, mem, size);
  return p;
}


void *mem_growvec(void *src, void *new, int stride, int c, int nc) {
  void *n = malloc(stride*(c+nc));
  if (!n)
    return NULL;
  memset(n, 0, stride*(c+nc));
  if (src)
    memcpy(n, src, stride*c);
  memcpy((char*)n + stride*c, new, stride*nc);
  return n;
}
