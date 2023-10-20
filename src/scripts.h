#pragma once

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

lua_State *lsc_prepstate();

int lsc_runscript(lua_State *L, const char *path);