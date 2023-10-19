#include "scripts.h"
#include "ozone.h"

/*
  char *var = getenv("HOME");
  lua_getglobal(L, "package");
  lua_getfield(L, -1, "path");
  std::string cpath = lua_tostring(L, -1);
  cpath.append(";").append(var) +=  "/.ozone/scripts/?.lua";
  lua_pop(L, 1);
  lua_pushstring(L, cpath.c_str());
  lua_setfield(L, -2, "path");
  lua_pop(L, 1);
*/

int main(int argc, char **argv) {
  _ENV env = {0};
  env.w = COLS;
  env.h = LINES;
  env.tabsize = 2;

  // TODO: Add utf8 support to the string helper functions

  int phi = utf8_literal('🔥');

  /*
  const char *yes = "🔥🔥";
  const long l = utf8_strlen(yes);
  printf("%li\n", l);
  printf("%li\n", utf8_actual(yes, 1));
  */
  
  resize_buffers(&env);


  /*

  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  set_escdelay(0);
  set_tabsize(env.tabsize);
  noecho();
  start_color();

  resize_buffers(&env);

  
  move(0, 0);
  refresh();

  int ch = 0;
  int rsz = 0;
  while ((ch = getch()) != 0x1b) {
    switch (ch) {
      case KEY_RESIZE: {
        resize_buffers(&env);
        ++rsz;
        break;
      }
      default:
      break;
    }
    mvprintw(0, 0, "%i, %i, %i, %i ", env.w, env.h, rsz, ch);
    move(0, 0);
    refresh();
  }

  move(0, 0);
  refresh();


  endwin();
  */




}