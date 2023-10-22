#include "lauxlib.h"
#include "lua.h"
#include "ozone.h"
#include "api.h"
#include "scripts.h"
#include "theme.h"

long l_intfield(lua_State *L, int off, const char *field) {
  if (!lua_istable(L, -1)) {
    luaL_error(L, "[OZAPI] expected table type on top of the stack");
    return 0;
  }
  lua_pushstring(L, field);
  lua_gettable(L, off-1);
  if (!lua_isinteger(L, -1)) {
    luaL_error(L, "[OZAPI] expected integer type on top of the stack");
    return 0;
  }
  long r = lua_tointeger(L, -1);
  lua_pop(L, 1);
  return r;
}

const char *l_strfield(lua_State *L, int indx, const char *field) {
  if (!lua_istable(L, -1)) {
    luaL_error(L, "[OZAPI] expected table type on top of the stack");
    return NULL;
  }
  lua_pushstring(L, field);
  lua_gettable(L, indx-1);
  if (!lua_isstring(L, -1)) {
    luaL_error(L, "[OZAPI] table %s property is not a string", field);
    return NULL;
  }
  const char *r = lua_tostring(L, -1);
  lua_pop(L, 1);
  return r;

}

int ozapi_addtheme(lua_State *L) {
  _ENV *env = (_ENV*)lua_tointeger(L, -2);
  if (!lua_istable(L, -1)) {
    luaL_error(L, "[OZAPI] expected table type on top of stack");
    return 0;
  }
  const char *name = l_strfield(L, -1, "name");
  if (!name)
    return 0;
  printf("New theme: %s\n", name);
  return 0;
}

int ozapi_fun(lua_State *L) {
  long ptr = lua_tointeger(L, -2);
  const char *str = lua_tostring(L, -1);
  printf("[API]_fun %li: %s\n", ptr, str);
  return 0;
}

int ozapi_setstyle(lua_State *L) {
  long ptr = lua_tointeger(L, -2);
  printf("[API]_setstyle %li:\n", ptr);
  _ENV *env = (_ENV*)ptr;
  

  int r = l_intfield(L, -2, "r");
  int g = l_intfield(L, -2, "g");
  int b = l_intfield(L, -2, "b");
  int r2 = l_intfield(L, -1, "r");
  int g2 = l_intfield(L, -1, "g");
  int b2 = l_intfield(L, -1, "b");
  printf("%i %i %i\n", r, g, b);
  return 0;
}
