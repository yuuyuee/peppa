// lvm

#include <iostream>
#include <string>
#include <cstring>
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

static const char* const kParsers = "_Parsers";
static const char* const kParse = "parse";
static const char* const kProtocol = "protocol";
static const char* const kDataItem = "dataitem";

#define PRINT_STACK_TOP() \
  std::cout << __LINE__ << " Top: " << lua_gettop(state) << std::endl

static void InitRuntimeEnv(lua_State* state) {
  assert(state != nullptr);
  lua_createtable(state, 0, 0);
  lua_setfield(state, LUA_REGISTRYINDEX, kParsers);
  lua_createtable(state, 0, 0);
  lua_setfield(state, LUA_REGISTRYINDEX, kDataItem);
}

static bool LoadModule(lua_State* state, const char* fname, std::string* err) {
  assert(state != nullptr && fname != nullptr);
  lua_getfield(state, LUA_REGISTRYINDEX, kParsers); // stack 1
  if (luaL_dofile(state, fname) == LUA_OK) { // stack 2
    if (lua_gettop(state) > 2)
      lua_pop(state, lua_gettop(state) - 2);

    if (!lua_istable(state, -1)) {
      if (err)
        *err = "Invalid parser format";
      lua_pop(state, 2);
      return false;
    }

    lua_getfield(state, -1, kProtocol);
    if (!lua_isstring(state, -1)) {
      if (err)
        *err = "Invalid protocol";
      lua_pop(state, 3);
      return false;
    }

    std::string protocol = lua_tostring(state, -1);
    lua_pop(state, 1);

    lua_getfield(state, -1, kParse);
    if (!lua_isfunction(state, -1)) {
      if (err)
        *err = "Invalid parse";
      lua_pop(state, 3);
      return false;
    }

    lua_pop(state, 1);
    lua_setfield(state, -2, protocol.c_str());
    lua_pop(state, 1);
    return true;
  }

  if (err)
    *err = lua_tostring(state, -1);
  lua_pop(state, 2);
  return false;
}

static void PrintDataItem(lua_State* state) {
  lua_getfield(state, LUA_REGISTRYINDEX, kDataItem);
  assert(lua_istable(state, -1));

  lua_pushnil(state);
  while (lua_next(state, -2)) {
    switch (lua_type(state, -2)) {
    case LUA_TNIL:
      std::cout << "(nil)";
      break;
    case LUA_TBOOLEAN:
      std::cout << (lua_toboolean(state, -2) ? "true" : "false");
      break;
    case LUA_TNUMBER:
      std::cout << lua_tonumber(state, -2);
      break;
    case LUA_TSTRING:
      std::cout << lua_tostring(state, -2);
      break;
    default:
      std::cout << "(other)";
      break;
    }

    std::cout << " = ";

    switch (lua_type(state, -1)) {
    case LUA_TNIL:
      std::cout << "(nil)";
      break;
    case LUA_TBOOLEAN:
      std::cout << (lua_toboolean(state, -1) ? "true" : "false");
      break;
    case LUA_TNUMBER:
      std::cout << lua_tonumber(state, -1);
      break;
    case LUA_TSTRING:
      std::cout << lua_tostring(state, -1);
      break;
    default:
      std::cout << "(other)";
      break;
    }

    std::cout << std::endl;
    lua_pop(state, 1);
  }
  lua_pop(state, 1);
}

static void PrintTable(lua_State* state, int idx,
    int tabs = 0, int stop_g = 0, int stop_index = 0) {
  lua_pushnil(state);
  while (lua_next(state, idx)) {
    for (int i = 0; i < tabs; ++i)
      std::cout << "  ";

    int stop_g_local = 0;
    int stop_index_local = 0;

    switch (lua_type(state, -2)) {
    case LUA_TNIL:
      std::cout << "(nil)";
      break;
    case LUA_TBOOLEAN:
      std::cout << (lua_toboolean(state, -2) ? "true" : "false");
      break;
    case LUA_TNUMBER:
      std::cout << lua_tonumber(state, -2);
      break;
    case LUA_TSTRING:
      stop_g_local = !strcmp(lua_tostring(state, -2), "_G");
      stop_index_local = !strcmp(lua_tostring(state, -2), "__index");
      std::cout << lua_tostring(state, -2);
      break;
    case LUA_TTABLE:
      std::cout << "(table)";
      break;
    case LUA_TLIGHTUSERDATA:
      std::cout << "(lightuserdata)";
      break;
    case LUA_TFUNCTION:
      std::cout << "(function)";
      break;
    case LUA_TUSERDATA:
      std::cout << "(userdata)";
      break;
    case LUA_TTHREAD:
      std::cout << "(thread)";
      break;
    default:
      std::cout << "(other)";
      break;
    }

    std::cout << " = ";

    switch (lua_type(state, -1)) {
    case LUA_TNIL:
      std::cout << "(nil)";
      break;
    case LUA_TBOOLEAN:
      std::cout << (lua_toboolean(state, -1) ? "true" : "false");
      break;
    case LUA_TNUMBER:
      std::cout << lua_tonumber(state, -1);
      break;
    case LUA_TSTRING:
      std::cout << lua_tostring(state, -1);
      break;
    case LUA_TTABLE:
      if ((stop_g && stop_g_local) || \
          (stop_index && stop_index_local) || \
          !strcmp(lua_tostring(state, -2), "loaded")) {
        std::cout << "{...}";
      } else {
        std::cout << "{" << std::endl;
        PrintTable(state, lua_gettop(state), tabs + 1, stop_g_local, stop_index_local);
        for (int i = 0; i < tabs; ++i)
          std::cout << "  ";
        std::cout << "}";
      }
      break;
    case LUA_TLIGHTUSERDATA:
      std::cout << "(lightuserdata)";
      break;
    case LUA_TFUNCTION:
      std::cout << "(function)";
      break;
    case LUA_TUSERDATA:
      std::cout << "(userdata)";
      break;
    case LUA_TTHREAD:
      std::cout << "(thread)";
      break;
    default:
      std::cout << "(other)";
      break;
    }

    std::cout << std::endl;
    lua_pop(state, 1);
  }
}

int main(int argc, char* argv[]) {
  LUAJIT_VERSION_SYM();

  if (argc != 2) {
    std::cout << argv[0] << " [script] [file]" << std::endl;
    return 0;
  }

  const char* script = argv[1];
  const char* fname = argv[2];

  lua_State* state = luaL_newstate();
  assert(state != NULL);

  // Load the standard libraries
  luaL_openlibs(state);

  // Initialize the runtime environ
  InitRuntimeEnv(state);
  assert(lua_gettop(state) == 0);

  std::string err;
  if (!LoadModule(state, script, &err)) {
    std::cout << "Error: " << err << std::endl;
    goto error;
  }

  // Push function 'parse' with protocol '998'
  assert(lua_gettop(state) == 0);
  lua_getfield(state, LUA_REGISTRYINDEX, kParsers);
  assert(lua_gettop(state) == 1 && lua_istable(state, -1));
  lua_getfield(state, -1, "998"); // parsers[protocol]
  assert(lua_gettop(state) == 2 && lua_istable(state, -1));
  lua_getfield(state, -1, kParse); // parsers[protocol][parse]
  assert(lua_gettop(state) == 3 && lua_isfunction(state, -1));

  // Push arguments
  lua_pushstring(state, "hello world");
  assert(lua_gettop(state) == 4);
  lua_getfield(state, LUA_REGISTRYINDEX, kDataItem);
  assert(lua_gettop(state) == 5);

  // Call function
  if (lua_pcall(state, 2, 0, 0) != LUA_OK) {
    const char* msg = lua_tostring(state, -1);
    std::cout << "Error: " << msg << std::endl;
    goto error;
  }
  assert(lua_gettop(state) == 2);
  lua_pop(state, 2);

  // Print results
  PrintDataItem(state);
  PRINT_STACK_TOP();

  // std::cout << "==== LUA_REGISTRYINDEX ====" << std::endl;
  // PrintTable(state, LUA_REGISTRYINDEX);
  // PRINT_STACK_TOP();

  // PrintTable(state, LUA_ENVIRONINDEX);
  // PRINT_STACK_TOP();

  // std::cout << "==== LUA_GLOBALSINDEX ====" << std::endl;
  // PrintTable(state, LUA_GLOBALSINDEX);
  // PRINT_STACK_TOP();

error:
  lua_close(state);
  return 0;
}