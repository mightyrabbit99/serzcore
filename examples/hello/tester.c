#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

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

static inline int test1() {
  printf("\n=== %s ===\n", __FUNCTION__);
  ssize_t res1;
  size_t len;
  struct hello_s *h1 = createdummy_hello_s();
  struct hello_s h2 = (struct hello_s){0};
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
  printf("deserializing data...\n");
  res1 = hello_deserialize(&h2, buf, len);
  if (res1 == -1) {
    lg("Read error!\n");
    return 1;
  }
  printf("data read:\n");
  printstru_hello_s(&h2, _fprintf, stdout, "h2.");
  delete_hello_s(h1);
  destroy_hello_s(&h2);
  return 0;
}

int main(int argc, char *argv[]) {
  test1();
  return 0;
}
