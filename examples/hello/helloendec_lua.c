#include "hello_stru.h"

#define SERZCORE_LUA
#include "lauxlib.h"
#include "lualib.h"
#include "serzcore_utils.h"

#define BUFFER_SIZE 100

#include "helloendecc.c"

int hello_decode_lua(lua_State *L) {
  size_t ll;
  size_t datasz;
  const char *data = lua_tolstring(L, -1, &datasz);
  char data2[datasz];
  memcpy(data2, data, datasz);
  lua_newtable(L);
  SZFREAD2(struct, hello_s, ll, data2, datasz, L);
  return 1;
}

int hello_encode_lua(lua_State *L) {
  SZFOUTEXEC(struct, hello_s, lua_pushlstring, L);
  return 1;
}

int hello_printw_lua(lua_State *L) {
  SZFPRINT_W2(struct, hello_s, L);
  return 0;
}

int hello_printr_lua(lua_State *L) {
  size_t datasz;
  const char *data = lua_tolstring(L, -1, &datasz);
  char data2[datasz];
  memcpy(data2, data, datasz);
  SZFPRINT_R2(struct, hello_s, data2, datasz, L);
  return 0;
}

// clang-format off
static const struct luaL_Reg mylib[] = {
    {"hello_decode", hello_decode_lua},
    {"hello_encode", hello_encode_lua},
    {"hello_printw", hello_printw_lua},
    {"hello_printr", hello_printr_lua},
    {NULL, NULL} /* sentinel */
};
// clang-format on

int luaopen_helloendec(lua_State *L) {
#if LUA_VERSION_NUM > 501
  luaL_newlib(L, mylib);
#else
  luaL_register(L, "helloendec_lua", mylib);
#endif
  return 1;
}
