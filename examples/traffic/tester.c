#include <ctype.h>
#include <stdio.h>

#include "trafficserz.h"

#define lg(...)                  \
  printf("[%s] ", __FUNCTION__); \
  printf(__VA_ARGS__)

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

static void _fprintf(void *file, const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  vfprintf(file, fmt, argp);
  va_end(argp);
}

static void _testtemplate_writeread_1(size_t tlvt_nr, vetyp_t *tlvts) {
  // write
  struct traffic_s *p = traffic_s_createdummy(tlvt_nr, tlvts);
  printf("start data:\n");
  traffic_s_print(p, _fprintf, stdout);
  uint8_t buffer[200];
  printf("serializing data...\n");
  ssize_t l = traffic_serialize(p, buffer, 200);
  if (l == -1) {
    lg("Write error!\n");
    return;
  }
  printf("data written:\n");
  hexdump(buffer, l);
  traffic_s_delete(p);

  // read
  struct traffic_s p2;
  printf("deserializing data...\n");
  l = traffic_deserialize(&p2, buffer, l);
  if (l == -1) {
    lg("Read error!\n");
    return;
  }
  printf("data read:\n");
  traffic_s_print(&p2, _fprintf, stdout);
  traffic_s_destroy(&p2);
}

#define _tlvts(arr) sizeof(arr) / sizeof(vetyp_t), arr
#define __testp1__(nr) printf("\n====================   TEST %d   ====================\n", nr)
#define __test1__(nr, ...)              \
  vetyp_t tlvts_##nr[] = {__VA_ARGS__}; \
  __testp1__(nr);                       \
  _testtemplate_writeread_1(_tlvts(tlvts_##nr))

static void test_writeread_1(void) {
  __test1__(1, vetyp_null);
  __test1__(2, vetyp_ship, vetyp_null);
  __test1__(3, vetyp_car, vetyp_ship, vetyp_null);
}

int main(int argc, char *argv[]) {
  test_writeread_1();
  return 0;
}
