#pragma once

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

lua_State *lsc_prepstate(void *env);

int lsc_runscript(lua_State *L, const char *path);