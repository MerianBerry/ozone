#include "lauxlib.h"
#include "lua.h"
#include "ozone.h"
#include "api.h"
#include "scripts.h"
#include "theme.h"

enum {
  lua_nil = 1,
  lua_int, 
  lua_str, 
  lua_num, 
  lua_tab, 
};


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

color_t hextocolor_t(const char *hex) {
  const char *r = str_substr(hex, 1, 2);
  const char *g = str_substr(hex, 3, 2);
  const char *b = str_substr(hex, 5, 2);
  color_t c = {0};
  c.r = strtol(r, NULL, 16);
  c.g = strtol(g, NULL, 16);
  c.b = strtol(b, NULL, 16);
  free((void*)r);
  free((void*)g);
  free((void*)b);
  return c;
}

int checkfield(lua_State *L, int indx, const char *field) {
  if (!lua_istable(L, indx))
      return 0;
  lua_pushstring(L, field);
  lua_gettable(L, indx-1);
  int t = 0;
  if (lua_isnil(L, -1))
    t = lua_nil;
  else if (lua_isinteger(L, -1))
    t = lua_int;
  else if (lua_isnumber(L, -1))
    t = lua_num;

  else if (lua_isstring(L, -1))
    t = lua_str;
  else if (lua_istable(L, -1))
    t = lua_tab;
  lua_pop(L, 1);
  return t;
}

style_t groupfield(lua_State *L, int indx, const char *field) {
  lua_pushstring(L, field);
  lua_gettable(L, indx-1);
  if (!lua_istable(L, -1)) {
    printf("[OZAPI] expected field %s to be a table\n", field);
    return (style_t){0};
  }
  const char *fgh = l_strfield(L, indx, "fg");
  const char *bgh = l_strfield(L, indx, "bg");
  printf("%s | fg: %s, bg: %s\n", field, fgh, bgh);
  color_t fg = hextocolor_t(fgh);
  color_t bg = hextocolor_t(bgh);
  style_t sty = {0};
  sty.fore = fg;
  sty.back = bg;
  return sty;
} 

int oz_addtheme(_ENV *env, int indx, lua_State *L) {
  theme_t t = {0};
  t.name = str_cpy(l_strfield(L, indx, "name"), npos);
  t.pairs = avl_newtree();
  style_t norm = groupfield(L, indx, "Normal"); 
  avl_append(t.pairs, "Normal", mem_todynamic(&norm, sizeof(norm)));

  void *oldthemev = env->themev;
  env->themev = mem_growvec(env->themev, &t, sizeof(theme_t), 
    env->themec, 1);
  free(oldthemev);

  return 0;
}

int ozapi_addplug(lua_State *L) {
  _ENV *env = (_ENV*)lua_tointeger(L, -2);
  if (!lua_istable(L, -1)) {
    printf("[OZAPIerr] (%p): expected table value for plugin\n", env);
    luaL_error(L, "[OSAPIerr], (%p): expected table value for plugin\n", (void*)env);
    return 0;
  }
  const char *type = l_strfield(L, -1, "type");
  if (!type)
    return 0;
  const char *name = l_strfield(L, -1, "name");
  if (!name)
    return 0;
  printf("[OZAPIcall]_addplug (%p): type: %s, name %s\n", env, type, name);
  switch (str_hash(type)) {
    case 210728967704: // theme
    oz_addtheme(env, -1, L);
    break;
  } 
  return 0;
}

int ozapi_addtheme(lua_State *L) {
  _ENV *env = (_ENV*)lua_tointeger(L, -2);
  if (!lua_istable(L, -1)) {
    luaL_error(L, "[OZAPI] expected table type on top of stack");
    return 0;
  }
  const char *name = l_strfield(L, -1, "name");
  if (!name) {
    return 0;
  }
  printf("New theme: %s\n", name);
  return 0;
}


