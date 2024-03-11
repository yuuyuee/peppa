// lvm

#include <iostream>
#include <cassert>

// #include <luajit-2.1/lua.hpp>
extern "C" {
#include <luajit-2.1/luaconf.h>
#include <luajit-2.1/lua.h>
#include <luajit-2.1/lauxlib.h>
#include <luajit-2.1/lualib.h>
#include <luajit-2.1/luajit.h>
}

// Call function in the C from C
// lua_pushcfunction(state, function_name)
// lua_putinteger(state, args1)
// lua_pcall(state, 1, 1, 0)
// auto res = lua_tointeger(state, -1)

// Call function in the C from lua
// lua_getglobal(state, "function_name")
// lua_pcall(state, 0, 1, 0)
// auto res = lua_tointeger(state, -1)

// Put an function to lua and call function in the lua
// lua_pushcfunction(state, function_name)
// lua_setglobal(state, "function_name")
// luaL_dostring(state, "function_name")

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << argv[0] << " [script] [file]" << std::endl;
    return 0;
  }

  const char* script = argv[1];
  const char* fname = argv[2];
  const char* protocol = "998";
  const char* func_name = "Parse";

  lua_State* state = luaL_newstate();
  assert(state != NULL);

  luaL_openlibs(state);

  lua_createtable(state, 0, 0);
  lua_setglobal(state, "parsers");

  if (luaL_dofile(state, script) != LUA_OK) {
    assert(lua_gettop(state) == 1);
    const char* msg = lua_tostring(state, -1);
    std::cout << "Error: " << msg << std::endl;
    lua_pop(state, lua_gettop(state));
    goto error;
  }
  assert(lua_gettop(state) == 0);
  lua_getglobal(state, "parsers");
  assert(lua_gettop(state) == 1 && lua_istable(state, -1));
  lua_getfield(state, -1, protocol); // parsers[protocol]
  assert(lua_gettop(state) == 2 && lua_istable(state, -1));
  lua_getfield(state, -1, func_name); // parsers[protocol][func_name]
  assert(lua_gettop(state) == 3 && lua_isfunction(state, -1));
  lua_pushstring(state, "hello world");
  assert(lua_gettop(state) == 4);
  lua_createtable(state, 0, 0);
  assert(lua_gettop(state) == 5);
  lua_setfield(state, LUA_REGISTRYINDEX, "dataitem");
  assert(lua_gettop(state) == 4);
  lua_getfield(state, LUA_REGISTRYINDEX, "dataitem");
  assert(lua_gettop(state) == 5);
  if (lua_pcall(state, 2, 0, 0) != LUA_OK) {
    const char* msg = lua_tostring(state, -1);
    std::cout << "Error: " << msg << std::endl;
    goto error;
  }
  assert(lua_gettop(state) == 2);
  lua_getfield(state, LUA_REGISTRYINDEX, "dataitem");
  assert(lua_gettop(state) == 3 && lua_istable(state, -1));
  lua_getfield(state, -1, "strm");
  assert(lua_gettop(state) == 4 && lua_isstring(state, -1));
  std::cout << "From C++: strm: " << lua_tostring(state, -1) << std::endl;
  lua_pop(state, 1);
  assert(lua_gettop(state) == 3 && lua_istable(state, -1));
  lua_getfield(state, -1, "app_type");
  assert(lua_gettop(state) == 4 && lua_isstring(state, -1));
  std::cout << "app_type: " << lua_tostring(state, -1) << std::endl;
error:
  lua_close(state);
  return 0;
}