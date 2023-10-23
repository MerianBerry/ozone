#include "theme.h"
#include <ncurses.h>
#include "ozone.h"

#define rgb8b(r, g, b) (short)(r/255.f*1000.f), (short)(g/255.f*1000.f), (short)(b/255.f*1000.f)

char compare_colors(color_t c1, color_t c2) {
  return c1.r==c2.r && c1.g==c2.g && c1.b==c2.b;
}
/*
int sty_pushstyle(theme_t *t, style_t s) {
  if (t->stylei >= 255)
    return -1;
  for (int i = 1; i < t->stylei; ++i) {
    if (compare_colors(t->styles[i].fore, s.fore)
    && compare_colors(t->styles[i].back, s.back)) {
      attron(COLOR_PAIR(i));
      t->current = t->styles[i];
      return i;
    }
  }
  t->styles[t->stylei] = s;
  init_color(t->stylei*2, rgb8b(s.fore.r, s.fore.g, s.fore.b));
  init_color(t->stylei*2+1, rgb8b(s.back.r, s.back.g, s.back.b));
  init_pair(t->stylei, t->stylei*2, t->stylei*2+1);
  attron(COLOR_PAIR(t->stylei));
  t->current = s;
  ++t->stylei;
}

void sty_resetstyles(theme_t *t) {
  t->stylei = 2;
  attron(COLOR_PAIR(plain_pair_e));
  t->current = t->styles[plain_pair_e];
}

void sty_swap(theme_t *t) {
  style_t s = sty_getstyle(t);
  color_t save = s.fore;
  s.fore = s.back;
  s.back = save;
  sty_pushstyle(t, s);
  t->current = s;
}

style_t sty_swapstyle(style_t s) {
  color_t save = s.fore;
  s.fore = s.back;
  s.back = save;
  return s;
}

color_t sty_getbackground(theme_t *t) {
  return t->styles[t->stylei-1].back;
}

color_t sty_getforeground(theme_t *t) {
  return t->styles[t->stylei-1].fore;
}

style_t sty_getstyle(theme_t *t) {
  return t->current;
}

void sty_initplain(theme_t *t) {
  t->stylei = 1;
  color_t f = t->fores[fore_plain_e];
  color_t b = t->backs[back_primary_e];
  t->styles[plain_pair_e] = (style_t){f, b};
  init_color(plain_fore_e, rgb8b(f.r, f.g, f.b));
  init_color(plain_back_e, rgb8b(b.r, b.g, b.b));
  init_pair(plain_pair_e, plain_fore_e, plain_back_e);
  bkgd(COLOR_PAIR(plain_pair_e));
  attron(COLOR_PAIR(plain_pair_e));
  t->current = t->styles[plain_pair_e];
  ++t->stylei;
}
*/
