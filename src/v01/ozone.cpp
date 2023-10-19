#include "ozone.hpp"
#include <ncurses.h>
#include <string>
#include <filesystem>

#include "lua.hpp"

#define OZONE_PATH "~/.ozone"

typedef struct _env {
  const char *local_path;
} _ENV;

enum {
  editor_back = 0x10,
  editor_var,
  editor_status_back,
  editor_info,

  pair_back_var = 0x10,

  pair_stat_info
};

void validcurse(int *x, int *y) {
  while (*x >= COLS) {
    (*x) = (*x) - COLS;
    ++(*y);
  }
  while (*x < 0) {
    (*x) = COLS-1;
    --(*y);
  }
  while (*y < 0) {
    (*y) = 0;
    (*x) = 0;
  }
  while (*y >= LINES) {
    (*y) = LINES-1;
  }
}

class Buffer {
  public:
  int*          buf = nullptr;
  int           w = 0;
  int           h = 0;

  void init(int _w, int _h) {
    w = _w;
    h = _h;
    buf = (int*)malloc(sizeof(int)*w*h);
    memset(buf, 0, sizeof(int)*w*h);
  }

  void resize(int _w, int _h) {
    if (_w == w && _h == h)
      return;
    int *nbuf = (int*)malloc(sizeof(int)*_w*_h);
    memset(nbuf, 0, sizeof(int)*_w*_h);
    memcpy(nbuf, buf, std::min(sizeof(int)*_w*_h, sizeof(int)*w*h));
    free(buf);
    buf = nbuf;
    w = _w;
    h = _h;
  }

  void setc(int x, int y, int c) {
    validcurse(&x, &y);
    x = std::min(abs(x), w);
    y = std::min(abs(y), h);
    buf[x + y*w] = c;
  }

  const char *emitzeros() const {
    const size_t bsz = sizeof(int)*COLS*LINES;
    char *str = (char*)malloc(bsz+1);
    memset(str, 0, bsz+1);
    /*for (long i = 0; i < COLS*LINES; ++i) {
      buf[i] = 0x00bd83ef;
    }*/
    long i2 = 0;
    for (long i = 0; i < bsz; ++i) {
      if (((char*)buf)[i]) {
        str[i2++] = ((char*)buf)[i];
      }
    }
    return str;
  }

  int endx(int y, int max) {
    for (int i = max; i >= 0; --i) {
      int c = buf[i+y*w];
      if (c) {
        return i+1;
      }
    }
    return 0;
  }

  int getc(int x, int y) const {
    return buf[x+y*w];
  }
};

static Buffer buf;

int unisize(u_char utf8c) {
  if (utf8c >= 0xf0 && utf8c <= 0xf4)
    return 4;
  else if (utf8c >= 0xe0)
    return 3;
  else if (utf8c >= 0xc2)
    return 2;
  return 1;
}


int draw(int x, int y) {
  buf.resize(COLS, LINES);
  attron(COLOR_PAIR(pair_back_var));
  const char *str = buf.emitzeros();
  size_t s = strlen(str);
  
  mvprintw(0, 0, "%s", str);
  

  attron(COLOR_PAIR(pair_stat_info));
  char *infstr = (char*)malloc(COLS+1);
  memset(infstr, 0, COLS+1);
  memset(infstr, ' ', COLS);
  mvprintw(LINES-1, 0, "Press q to exit %i, %i;%s", x, y, infstr);
  free((void*)infstr);

  free((void*)str);
  return 0;
}

#define rgb8b(r, g, b) (short)(r/255.f*1000.f), (short)(g/255.f*1000.f), (short)(b/255.f*1000.f)

class ozone
{
private:
  FILE *log = nullptr;
public:
  void prep();
  ozone(/* args */);
  ~ozone();
};

std::string fixhome(std::string path) {
  char *var = getenv("HOME");
  if (!var)
    return path;
  size_t i = path.find_first_of('~');
  if (i == std::string::npos)
    return path;
  path.replace(i, 1, var);
  return path;
}


bool checkdir(std::string path) {
  namespace fs = std::filesystem;
  path = fixhome(path);
  if (!fs::exists(path.c_str())) {
    fs::create_directory(path.c_str());
    return 0;
  }
  return 1;
}

FILE *openfile(const char *mode, const char *path) {
  return fopen(fixhome(path).c_str(), mode);
}


void ozone::prep() {
  
  checkdir(OZONE_PATH);
  checkdir(OZONE_PATH"/scripts");
  checkdir(OZONE_PATH"/scripts/editor");
  checkdir(OZONE_PATH"/scripts/custom");
  
  FILE *log = openfile(OZONE_PATH"log.txt", "w");
  if (!log)
    return;
}

ozone::ozone(/* args */)
{
}

ozone::~ozone()
{
  if (log)
    fclose(log);
}

int loadlua(lua_State *L, std::string path) {
  char *var = getenv("HOME");
  lua_getglobal(L, "package");
  lua_getfield(L, -1, "path");
  std::string cpath = lua_tostring(L, -1);
  cpath.append(";").append(var) +=  "/.ozone/scripts/?.lua";
  lua_pop(L, 1);
  lua_pushstring(L, cpath.c_str());
  lua_setfield(L, -2, "path");
  lua_pop(L, 1);

  path = fixhome(path);
  if (!std::filesystem::exists(path.c_str())) {
    fprintf(stderr, "%s is missing\n", path.c_str());
      return -2;
  }
  int ret = luaL_dofile(L, path.c_str());
  return ret;
}


static int l_apif(lua_State *L) {
  const char *s = lua_tostring(L, 1);
  fprintf(stderr, "%s\n", s);
  return 0;
}

int main(int argc, char **argv)
{
  
  ozone oz;
  oz.prep();
  

  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  lua_register(L, "oz_apif", l_apif);
  
  loadlua(L, "~/.ozone/scripts/config.lua");
  

  


  return 0;
  #if 0
  /*
  const u_char str[] = {0xef, 0x83, 0xbd, '\n', '\0'};
  fwrite(str, sizeof(str)-1, 1, stdout);
  printf("\n");
  */


  int ch;
  int x = 0;
  int y = 0;

  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  set_escdelay(0);
  noecho();
  start_color();
  set_tabsize(2);

  init_color(editor_back, rgb8b(43, 36, 37));
  init_color(editor_var, 1000, 300, 400);
  init_color(editor_status_back, rgb8b(39, 36, 43));
  init_color(editor_info, rgb8b(37, 186, 104));

  init_pair(pair_back_var, editor_var, editor_back);
  init_pair(pair_stat_info, editor_info, editor_status_back);

  bkgd(COLOR_PAIR(pair_back_var));

  int w = COLS;
  int h = LINES;

  buf.init(w, h);

  refresh();
  //win = create_newwin(h, w, y, x);
  draw(x, y);
  move(y, x);
  refresh();

  while((ch = getch()) != 0x1b)
  {
    switch (ch)
    {
      case KEY_RESIZE: {
        x = x + y*w;
        y = 0;
        w = COLS;
        h = LINES;
        break;
      }
      case KEY_BACKSPACE:
        buf.setc(x-1, y, 0x00);
        --x;
        validcurse(&x, &y);
        x = buf.endx(y, x-1);
        break;
      case '\n':
        buf.setc(x, y, '\n');
        x = 0;
        y++;
        validcurse(&x, &y);
        break;
      case KEY_LEFT:
        x--;
        validcurse(&x, &y);
        x = buf.endx(y, x-1);
        break;
      case KEY_RIGHT: {
        int oc = buf.getc(x, y);
        if (oc) {
          x++;
        } else {
          ++y;
          x = buf.endx(y, COLS);
        }
        break;
      }
        
        
      case KEY_UP:
        y--;
        validcurse(&x, &y);
        x = buf.endx(y, COLS-1);
        break;
      case KEY_DOWN:
        y++;
        validcurse(&x, &y);
        x = buf.endx(y, COLS-1);
        break;
      default:
        buf.setc(x, y, ch);
        ++x;
    }
    validcurse(&x, &y);

    draw(x, y);

    move(y, x);
    refresh();
  }

  endwin();
  free(buf.buf);
  return 0;
  #endif
}