#pragma once

#include <stddef.h>
#include <stdint.h>

enum {
  back_primary_e,
  back_secondary_e,
  back_highlight_e,
  back_extra_e,

  fore_plain_e = 0,
  fore_red_e,
  fore_green_e,
  fore_blue_e,
  fore_yellow_e,
  fore_cyan_e,
  fore_purple_e,
  fore_orange_e,
  fore_magenta_e,
  fore_custom_e,

  plain_fore_e = 1,
  plain_back_e,
  plain_pair_e = 1,
};

typedef struct color_s {
  uint8_t         r;
  uint8_t         g;
  uint8_t         b;
} color_t;

typedef struct style_s {
  color_t         fore;
  color_t         back;
} style_t;

typedef struct theme_s {
  color_t         backs[4];
  color_t         fores[64];
  style_t         styles[254];
  int             stylei;
  style_t         current;
} theme_t;

int sty_pushstyle(theme_t *t, style_t s);

void sty_resetstyles(theme_t *t);

color_t sty_getbackground(theme_t *t);

color_t sty_getforeground(theme_t *t);

style_t sty_getstyle(theme_t *t);

void sty_swap(theme_t *t);

style_t sty_swapstyle(style_t s);

void sty_initplain(theme_t *t);
