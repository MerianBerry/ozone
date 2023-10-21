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
  env.doubleclickt = 500.f;
  env.theme.fores[fore_plain_e] = (color_t){255, 80, 118};
  env.theme.backs[back_primary_e] = (color_t){43, 36, 37};

  io_mkdir("~/.ozone/test");

  lua_State *L = lsc_prepstate();
  lsc_runscript(L, "~/.ozone/scripts/config.lua");

  
  

  // TODO: Add utf8 support to the string helper functions
  
  //resize_buffers(&env);
  

  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  set_escdelay(0);
  set_tabsize(env.tabsize);
  noecho();
  start_color();
  // Don't mask any mouse events
  mousemask(BUTTON1_PRESSED | BUTTON1_RELEASED, NULL);
  mouseinterval(0);

  sty_initplain(&env.theme);
  
  resize_buffers(&env);

  
  move(0, 0);
  refresh();

  int ch = 0;
  int rsz = 0;
  int x, y = 0;
  timepoint_t lclick;
  while ((ch = getch()) != 0x1b) {
    sty_initplain(&env.theme);
    move(0, 0);
    switch (ch) {
      case KEY_RESIZE: {
        resize_buffers(&env);
        ++rsz;
        break;
      }
      case KEY_MOUSE: {
        MEVENT mev = {0};
        if (getmouse(&mev) == OK) {
          ch = mev.bstate;
          ++rsz;
          mvprintw(0, 0, "%i, %i\n", y-mev.y, x-mev.x);
          if (mev.bstate == 2) {
            mvprintw(3, 0, "Click!\n");
          } else {
            mvprintw(3, 0, "No click!\n");
          }
          if (mev.bstate == 2) {
            if (timeduration(timenow(), lclick, milliseconds_e) < env.doubleclickt
            && abs(y-mev.y)<=2 && abs(x-mev.x)<=2) {
              style_t old = sty_getstyle(&env.theme);
              style_t sg = old;
              sg.fore = (color_t){37, 186, 104};
              sty_pushstyle(&env.theme, sg);
              mvprintw(1,0,"Double click!\n");
              sty_pushstyle(&env.theme, old);
              
            } else {
              mvprintw(1,0,"Single click!\n");
            }
            lclick = timenow();
            y = mev.y;
            x = mev.x;
          } else {
            
          }
        }
        break;
      }
      default:
      break;
    }
    
    mvprintw(2, 0,"%i, %i, %i, %i ", env.w, env.h, rsz, ch);
    move(y, x);
    refresh();
    //sty_resetstyles(&env.theme);
  }


  endwin();
  




}