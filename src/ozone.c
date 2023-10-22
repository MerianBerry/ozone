#include "scripts.h"
#include "ozone.h"

#if !defined(MAX)
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif
#if !defined(MIN)
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif
#if !defined(CLAMP)
#define CLAMP(x, y, z) MAX(y, MIN(z, x))
#endif


int main(int argc, char **argv) {
  _ENV env = {0};
  env.w = COLS;
  env.h = LINES;
  env.tabsize = 2;
  env.doubleclickt = 500.f;
  env.theme.fores[fore_plain_e] = (color_t){80, 82, 84};
  env.theme.backs[back_primary_e] = (color_t){36, 36, 35};
  env.theme.backs[back_highlight_e] = (color_t){51, 52, 54};
  env.theme.fores[fore_green_e] = (color_t){37, 186, 104};

  setlocale(LC_CTYPE, "");

  io_mkdir("~/.ozone/test");

  lua_State *L = lsc_prepstate(&env);
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

  #if 0
  FIELD *field[3];
  FORM  *my_form;

  /* Initialize the fields */
  field[0] = new_field(1, 10, 4, 18, 0, 0);
  field[1] = new_field(1, 10, 6, 18, 0, 0);
  field[2] = NULL;

  /* Set field options */
  set_field_back(field[0], A_UNDERLINE);  /* Print a line for the option  */
  field_opts_off(field[0], O_AUTOSKIP);   /* Don't go to next field when this */
                      /* Field is filled up       */
  set_field_back(field[1], A_UNDERLINE); 
  field_opts_off(field[1], O_AUTOSKIP);

  /* Create the form and post it */
  my_form = new_form(field);
  post_form(my_form);
  #endif
  
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
  char lclickstate = 0;
  do {
    sty_initplain(&env.theme);
    {
      style_t edge = {0};
      edge.back = env.theme.backs[back_highlight_e];
      edge.fore = (color_t){116, 122, 138};
      style_t old = sty_getstyle(&env.theme);
      style_t edgeo = edge;
      sty_pushstyle(&env.theme, edge);
      for (int i = 0; i < LINES-2; ++i) {
        mvprintw(i, 0, "  \n");
      }
      for (int i = 0; i < COLS; ++i) {
        mvprintw(LINES-1, i, " ");
      }
      edge.back = env.theme.backs[back_primary_e];
      sty_pushstyle(&env.theme, edge);
      for (int i = 0; i < LINES-2; ++i) {
        mvprintw(i, 2, "%i\n", i+1);
      }
      edge.back = env.theme.fores[fore_green_e];
      edge.fore = (color_t){41, 48, 38};
      sty_pushstyle(&env.theme, edge);
      mvprintw(LINES-1, 0, " NORMAL ");
      style_t schar = sty_swapstyle(sty_getstyle(&env.theme));
      schar.back = env.theme.backs[back_highlight_e];
      sty_pushstyle(&env.theme, schar);
      char *s = utf8_tostring(utf8_encode(0x25E4));
      addstr(s);
      free(s);
      sty_pushstyle(&env.theme, old);
    }
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
          mvprintw(0, 5, "%i, %i\n", y-mev.y, x-mev.x);
          if (mev.bstate == 2) {
            mvprintw(3, 5, "Click!\n");
          } else {
            mvprintw(3, 5, "No click!\n");
          }
          if (mev.bstate == 2) {
            if (timeduration(timenow(), lclick, milliseconds_e) < env.doubleclickt
            && abs(y-mev.y)<=2 && abs(x-mev.x)<=2
            && !lclickstate) {
              style_t old = sty_getstyle(&env.theme);
              style_t sg = old;
              sg.fore = env.theme.fores[fore_green_e];
              sty_pushstyle(&env.theme, sg);
              mvprintw(1,5,"Double click!\n");
              sty_pushstyle(&env.theme, old);
              lclick = (timepoint_t){0};
              lclickstate = 1;
            } else {
              mvprintw(1,5,"Single click!\n");
              lclickstate = 0;
            }
            lclick = timenow();
            y = CLAMP(mev.y, 0, LINES-10);
            x = mev.x;
          } else {
            
          }
        }
        break;
      }
      default:
      break;
    }
    
    mvprintw(2, 5,"%i, %i, %i, %i ", env.w, env.h, rsz, ch);
    //printw("\n");
    move(y, x);
    refresh();
    //sty_resetstyles(&env.theme);
  } while ((ch = getch()) != 0x1b);


  #if 0
  /* Un post form and free the memory */
  unpost_form(my_form);
  free_form(my_form);
  free_field(field[0]);
  free_field(field[1]); 
  #endif
  endwin();
  




}