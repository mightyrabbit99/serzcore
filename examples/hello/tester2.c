#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include "base64.h"
#include "lauxlib.h"
#include "lualib.h"

#include "helloendec.h"

#define lg(...)                    \
  do {                             \
    printf("[%s] ", __FUNCTION__); \
    printf(__VA_ARGS__);           \
  } while (0)

static void _fprintf(void *file, const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  vfprintf(file, fmt, argp);
  va_end(argp);
}

void hexdump(const void *d, size_t datalen) {
  const uint8_t *data = d;
  size_t i, j = 0;

  for (i = 0; i < datalen; i += j) {
    printf("%4zu: ", i);
    for (j = 0; j < 16 && i + j < datalen; j++) printf("%02x ", data[i + j]);
    while (j++ < 16) printf("   ");
    printf("|");
    for (j = 0; j < 16 && i + j < datalen; j++) putchar(isprint(data[i + j]) ? data[i + j] : '.');
    printf("|\n");
  }
}

static inline int _test2_lua(uint8_t *buf, size_t len, const char *lua_program_1) {
  int res;
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  res = luaL_loadfile(L, lua_program_1);
  if (res != LUA_OK) {
    lg("lua error! %s\n", lua_tostring(L, -1));
    return 1;
  }
  res = lua_pcall(L, 0, LUA_MULTRET, 0);
  if (res != LUA_OK) {
    lg("Failed to run script: %s\n", lua_tostring(L, -1));
    return 1;
  }

  lua_getglobal(L, "test2");
  if (!lua_isfunction(L, -1)) {
    lg("ERROR: Not a function!\n");
    return 1;
  }

  // prepare arguments for test2()
  lua_pushlstring(L, buf, len);

  // call function (1 arguments, 1 result)
  res = lua_pcall(L, 1, 1, 0);
  if (res != LUA_OK) {
    lg("Error running lua: %s\n", lua_tostring(L, -1));
    return 1;
  }

  lua_close(L);
  return 0;
}

static inline int test2(const char *lua_program_1) {
  printf("\n=== %s ===\n", __FUNCTION__);
  if (lua_program_1 == NULL) {
    lg("Lua program needed!\n");
    return 1;
  }

  ssize_t res1;
  size_t len;
  struct hello_s *h1 = createdummy_hello_s();
  uint8_t buf[100];
  printstru_hello_s(h1, _fprintf, stdout, "h1->");
  printf("serializing data...\n");
  res1 = hello_serialize(h1, buf, sizeof(buf));
  if (res1 == -1) {
    lg("Write error!\n");
    return 1;
  }
  len = res1;
  printf("data written:\n");
  hexdump(buf, len);
  printf("data written (base64):\n");
  size_t bufb64_len = 0;
  uint8_t *bufb64 = base64_encode(buf, len, &bufb64_len);
  for (size_t i = 0; i < bufb64_len; i++) printf("%c", bufb64[i]);
  printf("\n");
  //_test2_lua(buf, len, lua_program_1);
  free(bufb64);
  delete_hello_s(h1);
}

int main(int argc, char *argv[]) {
  test2(argv[1]);
  return 0;
}
