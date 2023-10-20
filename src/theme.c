#include "theme.h"
#include <ncurses.h>

#define rgb8b(r, g, b) (short)(r/255.f*1000.f), (short)(g/255.f*1000.f), (short)(b/255.f*1000.f)

void sty_setbackground(style_t *s, color_t c) {
  s->back = c;
  init_color(active_back_e, rgb8b(c.r, c.g, c.b));
  init_pair(active_pair_e, active_fore_e, active_back_e);
  attron(COLOR_PAIR(active_pair_e));
}

void sty_setforeground(style_t *s, color_t c) {
  s->fore = c;
  init_color(active_fore_e, rgb8b(c.r, c.g, c.b));
  init_pair(active_pair_e, active_fore_e, active_back_e);
  attron(COLOR_PAIR(active_pair_e));
}

void sty_swap(style_t *s) {
  init_color(active_fore_e, rgb8b(s->back.r, s->back.g, s->back.b));
  init_color(active_back_e, rgb8b(s->fore.r, s->fore.g, s->fore.b));
  init_pair(active_pair_e, active_fore_e, active_back_e);
  attron(COLOR_PAIR(active_pair_e));
  const color_t save = s->fore;
  s->fore = s->back;
  s->back = save;
}

void sty_initplain(theme_t *t) {
  color_t b = t->backs[back_primary_e];
  color_t f = t->fores[fore_plain_e];
  init_color(plain_back_e, rgb8b(b.r, b.g, b.b));
  init_color(plain_fore_e, rgb8b(f.r, f.g, f.b));
  init_pair(plain_pair_e, plain_fore_e, plain_back_e);
  bkgd(COLOR_PAIR(plain_pair_e));
  init_color(active_fore_e, rgb8b(f.r, f.g, f.b));
  init_color(active_back_e, rgb8b(b.r, b.g, b.b));
  init_pair(active_pair_e, active_fore_e, active_back_e);
  attron(COLOR_PAIR(active_pair_e));
}