#include "serzcore.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MAZ_SZ (~(size_t)0)

static inline uint8_t bb_mask(uint8_t target, uint8_t p1, uint8_t p2, char off1) {
  if (p1 >= p2) return (uint8_t)0;
  uint16_t a = (1 << (8 - p1)) - (1 << (8 - p2));
  return (target & a) >> off1;
}

static inline void _szcpy(uint8_t typ, uint8_t *dst, uint8_t *src, size_t count, uint8_t pos_bb) {
  int i;
  switch (typ) {
    case cdef_SZ_o2:
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      for (i = 0; i < count; i++) {
        dst[i] = src[i];
      }
#else
      for (i = 0; i < count; i++) {
        dst[i] = src[count - i - 1];
      }
#endif
      break;
    case cdef_SZ_o:
      for (i = 0; i < count; i++) {
        dst[i] = src[i];
      }
      break;
    case cdef_SZ_b:
      uint8_t bb_left = 8 - pos_bb;
      for (i = 0; count > 8; count -= 8) {
        dst[i] &= bb_mask(255, 0, pos_bb, 0);
        dst[i] |= bb_mask(src[i], 0, bb_left, pos_bb);
        dst[i + 1] = 0;
        dst[i + 1] |= bb_mask(src[i], bb_left, 8, -bb_left);
        i++;
      }
      uint8_t x_1 = MIN(bb_left, count);
      dst[i] &= bb_mask(255, 0, pos_bb, 0);
      dst[i] |= bb_mask(src[i], 0, x_1, pos_bb);
      count -= x_1;

      if (count > 0) {
        dst[i + 1] = 0;
        dst[i + 1] |= bb_mask(src[i], x_1, x_1 + count, -bb_left);
      }
      break;
    default:
      break;
  }
}

// clang-format off
#include "serzcore.read.c"
#include "serzcore.write.c"
// clang-format on
