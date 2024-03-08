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

static int Multiplication(lua_State* state) {
  lua_Integer a = luaL_checkinteger(state, 1);
  lua_Integer b = luaL_checkinteger(state, 2);
  lua_Integer c = a * b;
  lua_pushinteger(state, c);
  return 1;
}

int main(int argc, char* argv[]) {
  lua_State* state = luaL_newstate();
  assert(state != NULL);

  luaL_openlibs(state);

  // lua_register(L, "mul", Multiplication);
  lua_pushcfunction(state, Multiplication);
  assert(lua_gettop(state) == 1);
  assert(lua_type(state, -1) == LUA_TFUNCTION);
  // std::cout << "Top = " << lua_gettop(state) << std::endl;
  // std::cout << "Top value type: " << luaL_typename(state, -1) << std::endl;
  // lua_pushinteger(state, 5);
  // lua_pushinteger(state, 8);
  lua_setglobal(state, "mul");
  // if (lua_pcall(state, 2, 1, 0) == LUA_OK) {
  //   auto top = lua_gettop(state);
  //   assert(top == 1);
  //   std::cout << "Top = " << top << std::endl;
  //   lua_Integer res = lua_tointeger(state, 1);
  //   std::cout << "result " << res << std::endl;
  //   lua_pop(state, top);
  //   top = lua_gettop(state);
  //   assert(top == 0);
  //   std::cout << "Top = " << top << std::endl;
  // }
  if (luaL_dostring(state, "return mul(5, 8)") == LUA_OK) {
    assert(lua_gettop(state) == 1);
    lua_Integer res = lua_tointeger(state, 1);
    std::cout << "result " << res << std::endl;
    lua_pop(state, lua_gettop(state));
  } else {
    auto s = lua_tostring(state, -1);
    std::cout << "Error messages: " << s << std::endl;
    lua_pop(state, lua_gettop(state));
  }

  lua_close(state);
  return 0;
}