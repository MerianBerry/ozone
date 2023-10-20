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

  active_back_e = 0x10,
  active_fore_e,
  plain_back_e,
  plain_fore_e,
  active_pair_e = 1,
  plain_pair_e,
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
} theme_t;

void sty_setbackground(style_t *s, color_t c);

void sty_setforeground(style_t *s, color_t c);

void sty_swap(style_t *s);

void sty_initplain(theme_t *t);
