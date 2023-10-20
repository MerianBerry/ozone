#include "scripts.h"
#include "ozone.h"

lua_State *lsc_prepstate() {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  lua_getglobal(L, "package");
  lua_getfield(L, -1, "path");
  char *cpath = (char*)str_cpy(lua_tostring(L, -1), npos);
  char *fpath = io_fixhome("~/.ozone/scripts/?.lua");
  cpath = str_append(cpath, ";", 1);
  cpath = str_append(cpath, fpath, npos);
  free(fpath);
  lua_pop(L, 1);
  lua_pushstring(L, cpath);
  lua_setfield(L, -2, "path");
  lua_pop(L, 1);
  free(cpath);
  return L;
}

int lsc_runscript(lua_State *L, const char *path) {
  char *fpath = io_fixhome(path);
  if (!io_exists(fpath)) {
    free(fpath);
    return -1;
  }
  int x = luaL_loadfile(L, fpath);
  int y = 0;
  if (!x) {
    y = lua_pcall(L, 0, LUA_MULTRET, 0);
  }
  free(fpath);
  return x || y;
}