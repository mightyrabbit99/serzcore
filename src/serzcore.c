/*
 * Copyright (c) 2025 Tan Wah Ken
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "serzcore.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifdef SERZCORE_LUA
#include "lauxlib.h"
#include "lualib.h"
#endif  // SERZCORE_LUA

#include "hashset.h"
#include "map.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MAZ_SZ (~(size_t)0)
#define zz8(x) ((x) == 0 ? 8 : (x))
#define zy8(x) ((x) % 8)

static void *(*szc_malloc)(size_t) = malloc;
static void *(*szc_realloc)(void *, size_t) = realloc;
static void (*szc_free)(void *) = free;

// clang-format off
const uint8_t byte_rev_table[256] = {
	0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
	0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
	0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
	0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
	0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
	0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
	0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
	0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
	0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
	0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
	0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
	0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
	0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
	0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
	0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
	0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
	0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
	0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
	0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
	0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
	0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
	0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
	0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
	0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
	0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
	0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
	0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
	0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
	0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
	0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
	0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
	0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff,
};
// clang-format on

static inline uint8_t bb_rev8(uint8_t byte) {
  return byte_rev_table[byte];
}

static inline uint8_t bb_mask(uint8_t target, char p1, char p2, char off1) {
  if (off1 >= 8) return (uint8_t)0;
  if (p1 < 0 || p2 < 0 || p1 >= p2) return (uint8_t)0;
  uint16_t a = (1 << (8 - p1)) - (1 << (8 - p2));
  return off1 > 0 ? (target & a) >> off1 : (target & a) << (-off1);
}

static inline uint8_t bb_mask2(uint8_t target, char p1, char p2, char off1) {
  if (off1 >= 8) return (uint8_t)0;
  if (p1 < 0 || p2 < 0 || p1 >= p2) return (uint8_t)0;
  uint16_t a = (1 << p2) - (1 << p1);
  return off1 > 0 ? (target & a) << off1 : (target & a) >> (-off1);
}

static inline void _szcpy_w(szc_dtyp_t typ, uint8_t *dst, const uint8_t *src, unsigned long long int count, uint8_t pos_bb) {
  uint8_t pos_ba = count % 8;
  uint8_t ba_left = 8 - pos_ba, bb_left = 8 - pos_bb;
  uint8_t x1 = MIN(bb_left, pos_ba), x2 = pos_ba - x1;
  size_t i, cnt2 = count >> 3;
  switch (typ) {
    case szc_dtyp_o:
      for (i = 0; i < count; i++) dst[i] = src[i];
      break;
    case szc_dtyp_o2:
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      for (i = 0; i < count; i++) dst[i] = src[i];
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
      for (i = 0; i < count; i++) dst[i] = src[count - i - 1];
#else
#error byte order not defined
#endif
      break;
    case szc_dtyp_o3:
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
      for (i = 0; i < count; i++) dst[i] = src[i];
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      for (i = 0; i < count; i++) dst[i] = src[count - i - 1];
#else
#error byte order not defined
#endif
      break;
    case szc_dtyp_b:
      for (i = 0; i < cnt2; i++) {
        dst[i] &= bb_mask(255, 0, pos_bb, 0);
        dst[i] |= bb_mask(src[i], 0, bb_left, pos_bb);
        if (bb_left < 8) {
          dst[i + 1] = 0;
          dst[i + 1] |= bb_mask(src[i], bb_left, 8, -bb_left);
        }
      }
      if (pos_bb > 0) {
        dst[i] &= bb_mask(255, 0, pos_bb, 0);
        dst[i] |= bb_mask(src[i], 0, x1, pos_bb);
      }

      if (x2 > 0) {
        dst[i + 1] = 0;
        dst[i + 1] |= bb_mask(src[i], x1, x1 + x2, -bb_left);
      }
      break;
    case szc_dtyp_b2:
      for (i = 0; i < cnt2; i++) {
        dst[i] &= bb_mask(255, 0, pos_bb, 0);
        dst[i] |= bb_mask(bb_rev8(src[i]), 0, bb_left, pos_bb);
        if (bb_left < 8) {
          dst[i + 1] = 0;
          dst[i + 1] |= bb_mask(bb_rev8(src[i]), bb_left, 8, -bb_left);
        }
      }
      if (pos_bb > 0) {
        dst[i] &= bb_mask(255, 0, pos_bb, 0);
        dst[i] |= bb_mask(bb_rev8(src[i]), 0, x1, pos_bb);
      }

      if (x2 > 0) {
        dst[i + 1] = 0;
        dst[i + 1] |= bb_mask(bb_rev8(src[i]), x1, x1 + x2, -bb_left);
      }
      break;
    case 101:
      cnt2 -= (pos_ba == 0 ? 1 : 0);
      dst[0] &= bb_mask2(255, bb_left, 8, 0);
      for (i = 0; i <= cnt2; i++) {
        dst[i] |= bb_mask2(bb_rev8(src[cnt2 - i]), 0, zz8(pos_ba), zy8(ba_left) - pos_bb);
        if (i == cnt2 && pos_ba - bb_left < 0) break;
        dst[i + 1] = 0;
        dst[i + 1] |= bb_mask2(bb_rev8(src[cnt2 - i]), 0, zz8(pos_ba) - zz8(bb_left), 8 - (zz8(pos_ba) - zz8(bb_left)));  // spillover
        if (i == cnt2) break;
        dst[i + 1] |= bb_mask(bb_rev8(src[cnt2 - i - 1]), 0, 8 - (zz8(pos_ba) - zz8(bb_left)), zz8(pos_ba) - zz8(bb_left));  // spillover
        dst[i] |= bb_mask(bb_rev8(src[cnt2 - i - 1]), 0, zz8(bb_left) - zz8(pos_ba), 8 - (zz8(bb_left) - zz8(pos_ba)));      // not enough
        dst[i + 1] |= bb_mask(bb_rev8(src[cnt2 - i - 1]), zz8(bb_left) - zz8(pos_ba), 8, zz8(pos_ba) - zz8(bb_left));        // not enough
      }
      break;
    default:
      break;
  }
}

static inline void _szcpy_r(szc_dtyp_t typ, uint8_t *dst, const uint8_t *src, unsigned long long int count, uint8_t pos_bb) {
  uint8_t pos_ba = count % 8, pos_bc = (pos_bb + pos_ba) % 8;
  uint8_t ba_left = 8 - pos_ba, bb_left = 8 - pos_bb, bc_left = 8 - pos_bc;
  uint8_t x1 = MIN(bb_left, pos_ba), x2 = pos_ba - x1;
  size_t i, cnt = (count + pos_bb) >> 3, cnt2 = count >> 3;
  switch (typ) {
    case szc_dtyp_o:
      for (i = 0; i < count; i++) dst[i] = src[i];
      break;
    case szc_dtyp_o2:
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      for (i = 0; i < count; i++) dst[i] = src[i];
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
      for (i = 0; i < count; i++) dst[i] = src[count - i - 1];
#else
#error byte order not defined
#endif
      break;
    case szc_dtyp_o3:
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
      for (i = 0; i < count; i++) dst[i] = src[i];
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      for (i = 0; i < count; i++) dst[i] = src[count - i - 1];
#else
#error byte order not defined
#endif
      break;
    case szc_dtyp_b:
      for (i = 0; i < cnt2; i++) {
        dst[i] |= bb_mask(src[i], pos_bb, 8, -pos_bb);
        dst[i] |= bb_mask(src[i + 1], 0, pos_bb, bb_left);
      }
      dst[i] |= bb_mask(src[i], pos_bb, pos_bb + x1, -pos_bb);
      if (pos_ba > bb_left)
        dst[i] |= bb_mask(src[i + 1], 0, pos_ba - bb_left, pos_ba);
      break;
    case szc_dtyp_b2:
      for (i = 0; i < cnt2; i++) {
        dst[i] |= bb_rev8(bb_mask(src[i], pos_bb, 8, -pos_bb));
        dst[i] |= bb_rev8(bb_mask(src[i + 1], 0, pos_bb, bb_left));
      }
      dst[i] |= bb_rev8(bb_mask(src[i], pos_bb, pos_bb + x1, -pos_bb));
      if (pos_ba > bb_left)
        dst[i] |= bb_rev8(bb_mask(src[i + 1], 0, pos_ba - bb_left, pos_ba));
      break;
    case 101:
      if (pos_bc > 0)
        dst[0] |= bb_mask2(bb_rev8(src[cnt]), 0, pos_bc, bc_left);
      for (i = 0; i < cnt2; i++) {
        dst[i] |= bb_mask(bb_rev8(src[cnt - i - 1]), 0, bc_left, pos_bc);
        dst[i + 1] |= bb_mask2(bb_rev8(src[cnt - i - 1]), 0, pos_bc, bc_left);
      }
      dst[i] |= bb_mask(bb_rev8(src[0]), 0, MIN(bb_left, zz8(bc_left)), pos_bc);
      if (bc_left < bb_left)
        dst[i + 1] |= bb_mask2(bb_rev8(src[0]), bb_left, pos_bc, bc_left);
      break;
  }
}

static inline int _szcmp(szc_dtyp_t typ, uint8_t *dst, const uint8_t *src, unsigned long long int count, uint8_t pos_bb) {
  uint8_t data[szc_count_oct(typ, count)];
  memset(data, 0, sizeof(data));
  _szcpy_r(typ, data, src, count, pos_bb);
  return memcmp(data, dst, szc_count_oct(typ, count));
}

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define _szcpy2(typ, dst, src, count, bbcnt) _szcpy_r(typ, (dst) + ((bbcnt) - szc_count_oct(typ, count)), src, count, 0)
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define _szcpy2(typ, dst, src, count, bbcnt) _szcpy_r(typ, dst, src, count, 0)
#else
#error byte order not defined
#endif
#define _szcpy3(typ, dst, src, count, dsttyp) _szcpy2(typ, dst, src, count, szc_conv_1(typ, sizeof(dsttyp)));

#define print_bits(f, arg, x)                                          \
  do {                                                                 \
    typeof(x) a__ = (x);                                               \
    char *p__ = (char *)&a__ + sizeof(x) - 1;                          \
    size_t bytes__ = sizeof(x);                                        \
    while (bytes__--) {                                                \
      char bits__ = 8;                                                 \
      while (bits__--) f(arg, "%c", *p__ & (1 << bits__) ? '1' : '0'); \
      p__--;                                                           \
    }                                                                  \
  } while (0)

static inline void _szcprint_d(void (*f)(void *, const char *format, ...), void *arg, szc_dtyp_t typ, const uint8_t *src, unsigned long long int count, uint8_t pos_bb) {
  size_t i, cnt2 = count >> 3;
  switch (typ) {
    case szc_dtyp_o:
      for (i = 0; i < count; i++) f(arg, i == 0 ? "%.2x" : " %.2x", src[i]);
      break;
    case szc_dtyp_o2:
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      for (i = 0; i < count; i++) f(arg, i == 0 ? "%.2x" : " %.2x", src[i]);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
      for (i = 0; i < count; i++) f(arg, i == 0 ? "%.2x" : " %.2x", src[count - i - 1]);
#else
#error byte order not defined
#endif
      break;
    case szc_dtyp_o3:
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
      for (i = 0; i < count; i++) f(arg, i == 0 ? "%.2x" : " %.2x", src[i]);
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      for (i = 0; i < count; i++) f(arg, i == 0 ? "%.2x" : " %.2x", src[count - i - 1]);
#else
#error byte order not defined
#endif
      break;
    case szc_dtyp_b:
      for (i = 0; i < cnt2; i++) print_bits(f, arg, src[i]);
      break;
    case szc_dtyp_b2:
      for (i = 0; i < cnt2; i++) print_bits(f, arg, src[cnt2 - i - 1]);
      break;
  }
}

static void _fprintf(void *file, const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  vfprintf(file, fmt, argp);
  va_end(argp);
}

static inline void _szcprint(void (*f)(void *, const char *format, ...), void *arg, szc_dtyp_t typ, const uint8_t *src, unsigned long long int count, uint8_t pos_bb, const char *name,
                             szc_extyp_t extyp, va_list extyp_va) {
  szc_extyp_t extyp2;
  int arr_i = -1;
  unsigned long long int val1 = 0;

  if (extyp == szc_extyp_arr) {
    extyp2 = va_arg(extyp_va, int);
    arr_i = va_arg(extyp_va, int);
  } else {
    extyp2 = extyp;
  }
  f(arg, "%s", name);
  if (arr_i != -1) f(arg, "[%d]", arr_i);
  if (extyp2 == szc_extyp_data) {
    if (szc_typ_is_octal(typ))
      f(arg, " (hex)");
    else
      f(arg, " (bin)");
  }
  f(arg, ": ");

  switch (extyp2) {
    case szc_extyp_string:
      f(arg, "%s", src);
      break;
    case szc_extyp_int:
      _szcpy3(szc_dtyp_o, ((uint8_t *)&val1), src, count, val1);
      f(arg, "%lld", val1);
      break;
    case szc_extyp_data:
      _szcprint_d(f, arg, typ, src, count, pos_bb);
      break;
  }
}

static void *szc_calloc(size_t num, size_t size) {
  if (szc_malloc == malloc) return calloc(num, size);
  void *ans = szc_malloc(num * size);
  memset(ans, 0, num * size);
  return ans;
}

void szc_set_mem_functions(void *(*malloc_fn)(size_t), void *(*realloc_fn)(void *, size_t), void (*free_fn)(void *)) {
  szc_malloc = malloc_fn;
  szc_realloc = realloc_fn;
  szc_free = free_fn;
  hashset_set_mem_functions(szc_calloc, szc_free);
}

#if defined(SERZCORE_LUA)
static inline int _szclua_r(lua_State *L, szc_extyp_t extyp, va_list extyp_va, const char *name, uint8_t *src, size_t src_sz) {
  int res = 0;
  szc_extyp_t extyp2;
  if (!lua_istable(L, -1)) return 1;

  if (extyp == szc_extyp_arr) {
    lua_getfield(L, -1, name);
    if (lua_isnil(L, -1)) {
      lua_pop(L, 1);
      lua_pushstring(L, name);
      lua_newtable(L);
      lua_settable(L, -3);
      lua_getfield(L, -1, name);
    }
    extyp2 = va_arg(extyp_va, int);
    size_t arrlen = va_arg(extyp_va, int);
    lua_pushnumber(L, arrlen + 1);
  } else {
    extyp2 = extyp;
    lua_pushstring(L, name);
  }

  switch (extyp2) {
    case szc_extyp_int:
    case szc_extyp_bigint:
      lua_pushnumber(L, *(int *)src);
      lua_settable(L, -3);
      break;
    case szc_extyp_data:
    case szc_extyp_string:
      lua_pushlstring(L, src, src_sz);
      lua_settable(L, -3);
      break;
    case szc_extyp_arrlen:
      break;
    default:
      res = 1;
      break;
  }

  if (extyp == szc_extyp_arr) {
    lua_pop(L, 1);
  }

  return res;
}

static inline ssize_t _szclua_w_get_fieldlen(lua_State *L, szc_extyp_t extyp, const char *name) {
  if (extyp >= _szc_extyp_max) return -1;
  if (!lua_istable(L, -1)) return -1;
  lua_getfield(L, -1, name);
  ssize_t res = -1;
  switch (extyp) {
    case szc_extyp_int:
    case szc_extyp_bigint:
    case szc_extyp_arrlen:
      break;
    case szc_extyp_data:
    case szc_extyp_string:
      lua_tolstring(L, -1, &res);
      break;
    default:
      break;
  }

  lua_pop(L, 1);
  return res;
}

struct _szclua_w_param_s {
  szcmode_t mode;
  void (*f)(void *, const char *format, ...);
  void *arg;
};

static inline int _szclua_w_do(struct _szclua_w_param_s param, lua_State *L, szc_extyp_t extyp, va_list extyp_va, const char *name, szc_dtyp_t typ, uint8_t **valp, unsigned long long int *bitlen,
                               size_t maxlen, unsigned long long int count) {
  szc_extyp_t extyp2;
  size_t start, end;
  size_t target2_len;
  const char *target2_str;
  int target2_int;
  uint8_t *val2;
  int arr_i = 0;

  if (!lua_istable(L, -1)) return 1;
  lua_getfield(L, -1, name);
  if (extyp == szc_extyp_arr) {
    extyp2 = va_arg(extyp_va, int);
    arr_i = va_arg(extyp_va, int);
    lua_rawgeti(L, -1, arr_i + 1);
  } else {
    extyp2 = extyp;
  }
  if (extyp2 >= _szc_extyp_max) return 1;

  start = *bitlen >> 3;
  if (szc_typ_is_octal(typ))
    end = start + count;
  else
    end = ((*bitlen + count) >> 3) + ((*bitlen + count) % 8 == 0 ? 0 : 1);
  switch (extyp2) {
    case szc_extyp_int:
    case szc_extyp_bigint:
      target2_int = (int)lua_tonumber(L, -1);
      val2 = szc_realloc(*valp, end);
      if (val2 == NULL) goto fail;
      *valp = val2;
      if (param.mode == szcmode_print)
        _szcprint(param.f, param.arg, typ, (uint8_t *)&target2_int, count, szc_typ_is_octal(typ) ? 0 : (*bitlen) % 8, name, extyp, extyp_va);
      else
        _szcpy_w(typ, val2 + (*bitlen >> 3), (uint8_t *)&target2_int, count, szc_typ_is_octal(typ) ? 0 : (*bitlen) % 8);
      *bitlen += szc_count_bit(typ, count);
      break;
    case szc_extyp_arrlen:
      target2_len = lua_objlen(L, -1);
      val2 = szc_realloc(*valp, end);
      if (val2 == NULL) goto fail;
      *valp = val2;
      if (param.mode == szcmode_print)
        _szcprint(param.f, param.arg, typ, (uint8_t *)&target2_len, count, szc_typ_is_octal(typ) ? 0 : (*bitlen) % 8, name, extyp, extyp_va);
      else
        _szcpy_w(typ, val2 + (*bitlen >> 3), (uint8_t *)&target2_len, count, szc_typ_is_octal(typ) ? 0 : (*bitlen) % 8);
      *bitlen += szc_count_bit(typ, count);
      break;
    case szc_extyp_data:
    case szc_extyp_string:
      target2_str = lua_tolstring(L, -1, &target2_len);
      if (param.mode == szcmode_print) {
        _szcprint(param.f, param.arg, typ, target2_str, target2_len, szc_typ_is_octal(typ) ? 0 : (*bitlen) % 8, name, extyp, extyp_va);
      } else {
        end = start + target2_len;
        if (end > maxlen) goto fail;
        val2 = szc_realloc(*valp, end);
        if (val2 == NULL) goto fail;
        *valp = val2;
        _szcpy_w(typ, val2 + (*bitlen >> 3), target2_str, target2_len, szc_typ_is_octal(typ) ? 0 : (*bitlen) % 8);
      }
      *bitlen += (target2_len << 3);
      break;
    default:
      break;
  }
  lua_pop(L, 1);
  if (extyp == szc_extyp_arr) {
    lua_pop(L, 1);
  }
  return 0;
fail:
  lua_pop(L, 1);
  if (extyp == szc_extyp_arr) {
    lua_pop(L, 1);
  }
  return 1;
}

static inline int _szclua_w_append(lua_State *L, szc_extyp_t extyp, va_list extyp_va, const char *name, szc_dtyp_t typ, uint8_t **valp, unsigned long long int *bitlen, size_t maxlen,
                                   unsigned long long int count) {
  return _szclua_w_do(
      (struct _szclua_w_param_s){
          .mode = szcmode_write,
      },
      L, extyp, extyp_va, name, typ, valp, bitlen, maxlen, count);
}

static inline int _szclua_w_print(void (*f)(void *, const char *format, ...), void *arg, lua_State *L, szc_extyp_t extyp, va_list extyp_va, const char *name, szc_dtyp_t typ, uint8_t **valp,
                                  unsigned long long int *bitlen, size_t maxlen, unsigned long long int count) {
  return _szclua_w_do(
      (struct _szclua_w_param_s){
          .mode = szcmode_print,
          .f = f,
          .arg = arg,
      },
      L, extyp, extyp_va, name, typ, valp, bitlen, maxlen, count);
}
#endif

// clang-format off
#if defined(SERZCORE_LUA)
#include "serzcore.read.lua.c"
#include "serzcore.write.lua.c"
#include "serzcore.print.lua.c"
#else
#include "serzcore.read.c"
#include "serzcore.write.c"
#include "serzcore.print.c"
#include "serzcore.free.c"
#endif
// clang-format on
