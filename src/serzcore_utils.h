/**
 * Copyright (c) 2025 mightyrabbit99
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `serzcore.c` for details.
 */

#ifndef SERZCORE_UTILS_H
#define SERZCORE_UTILS_H
#include "serzcore.h"

static inline uint8_t szc_get_msb32(register unsigned int val) { return 32 - __builtin_clz(val); }

static inline uint8_t szc_get_msb64(register unsigned long long val) { return 64 - __builtin_clzll(val); }

static inline uint8_t szc_get_ctnsz(register unsigned long long val) {
  uint8_t a = szc_get_msb64(val);
  return (a / 8) + (a % 8 > 0 ? 1 : 0);
}

#define szc_cvector_size(vec) ((vec) ? ((size_t *)(vec))[-1] : (size_t)0)
#define szc_cvector_set_size(vec, size) \
  do {                                  \
    if ((vec)) {                        \
      ((size_t *)(vec))[-1] = (size);   \
    }                                   \
  } while (0)
#define szc_cvector_free(vec)     \
  do {                            \
    if ((vec)) {                  \
      free(&((size_t *)vec)[-1]); \
    }                             \
  } while (0)
#define szc_cvector_grow(vec, count)                                                      \
  do {                                                                                    \
    const size_t cv_sz__ = sizeof(size_t) + (count) * sizeof(*(vec));                     \
    if ((vec) == NULL) {                                                                  \
      (vec) = (void *)&((size_t *)malloc(cv_sz__))[1];                                    \
      szc_cvector_set_size((vec), 0);                                                     \
    } else                                                                                \
      (vec) = (void *)&((size_t *)realloc(((void *)(vec)) - sizeof(size_t), cv_sz__))[1]; \
  } while (0)
#define szc_cvector_push_back(vec, value)                   \
  do {                                                      \
    szc_cvector_grow((vec), szc_cvector_size(vec) + 1);     \
    (vec)[szc_cvector_size(vec)] = (value);                 \
    szc_cvector_set_size((vec), szc_cvector_size(vec) + 1); \
  } while (0)
#define szc_cvector_for_each(vec, func)             \
  do {                                              \
    if ((vec) && (func) != NULL) {                  \
      size_t i;                                     \
      for (i = 0; i < szc_cvector_size(vec); i++) { \
        func((vec)[i]);                             \
      }                                             \
    }                                               \
  } while (0)

#define SZC_PASTER(x, y) x##_##y
#define SZC_CONCAT(x, y) SZC_PASTER(x, y)

#define szcthrowerr()                                   \
  do {                                                  \
    szc_cvector_for_each(*__dg_ptrs, szca__->szcfree2); \
    goto szcfail;                                       \
  } while (0)
#define _szcy_exec(f1, ...)        \
  do {                             \
    if (szca__->f1(__VA_ARGS__)) { \
      szcthrowerr();               \
    }                              \
  } while (0)
#define szcy(typ, count, target, d) _szcy_exec(szcy, typ, count, target, d)
#define szcyy(typ, count, target, d) _szcy_exec(szcyy, typ, count, (uint8_t *)(target), d)
#define szcyf(f, target_ex, d) _szcy_exec(szcyf, f, target_ex, d)
#define szcys_val(target, d) _szcy_exec(szcys_val, target, d)
#define szcmlc(target, sz) _szcy_exec(szcmlc, target, sz)
#define szcrealc(target, sz) _szcy_exec(szcrealc, target, sz)
#define szcmemset(s, c, sz) szca__->szcmemset(s, c, sz)
#define szcdelete(pt) szca__->szcfree(pt)
#define szc_get_mode() szca__->szc_get_mode()

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define szcyx(typ, count, bbcnt, target, d) szcyy(typ, count, target + (bbcnt - count), d)
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define szcyx(typ, count, bbcnt, target, d) szcyy(typ, count, target, d)
#else
#error byte order not defined
#endif
#define szcf(f, p, dst) SZFNAME(f)(szca__, p, dst)
#define szcmlcl(ptr, len)                              \
  do {                                                 \
    szcmlc((void **)(ptr), len);                       \
    szc_cvector_push_back(*__dg_ptrs, (void **)(ptr)); \
  } while (0)

#define szcmlcyy(typ, len, ptr, dst)        \
  do {                                      \
    szcmlcl(&(ptr), len);                   \
    szcyy(typ, len, (uint8_t *)(ptr), dst); \
  } while (0)
#define szclvp(typ, len, ptr, dst)                   \
  do {                                               \
    szcyy(typ, sizeof(len), (uint8_t *)&(len), dst); \
    szcmlcyy(typ, (len) * sizeof(*(ptr)), ptr, dst);   \
  } while (0)
#define szclvstr(typ, maxlen, ptr, dst)                                    \
  do {                                                                     \
    size_t tlv_len__ = (ptr) == NULL ? 0 : (strnlen(ptr, maxlen) + 1);     \
    if (tlv_len__ > maxlen) szcthrowerr();                                 \
    szcyx(typ, szc_get_ctnsz(maxlen), sizeof(tlv_len__), &tlv_len__, dst); \
    szcmlcyy(typ, tlv_len__, ptr, dst);                                    \
  } while (0)
#define szclvrcrse(typ, tlv_len_t, ff, target, dst)        \
  do {                                                     \
    struct szc_dgs_s *szc_tlvv__ = szca__->szc_init();     \
    szcyf(ff, target, szc_tlvv__);                         \
    tlv_len_t tlv_len__ = szca__->szc_get_len(szc_tlvv__); \
    szcyy(typ, sizeof(tlv_len__), &tlv_len__, dst);        \
    szca__->szc_set_maxlen(szc_tlvv__, tlv_len__);         \
    size_t l1 = szca__->szc_get_len(dst);                  \
    szcys_val(szc_tlvv__, dst);                            \
    size_t l2 = szca__->szc_get_len(dst);                  \
    szca__->szc_destruct(szc_tlvv__);                      \
    szc_tlvv__ = NULL;                                     \
    if (l2 - l1 != tlv_len__) szcthrowerr();               \
  } while (0)

#define SZFNAME(struname) SZC_CONCAT(__szcf, struname)
#define SZFNAME2(struname) SZC_CONCAT(__szcf2, struname)
#define SZFDECL(t__, struname, p, dst)                                                                                              \
  static inline int SZFNAME2(struname)(const struct szc_dga_s *szca__, t__ struname *p, struct szc_dgs_s *dst, void ****__dg_ptrs); \
  int SZFNAME(struname)(const struct szc_dga_s *szca__, void *p, struct szc_dgs_s *dst) {                                           \
    void ***ptrs = NULL;                                                                                                            \
    int ans = SZFNAME2(struname)(szca__, (t__ struname *)p, dst, &ptrs);                                                            \
    szc_cvector_free(ptrs);                                                                                                         \
    return ans;                                                                                                                     \
  }                                                                                                                                 \
  static inline int SZFNAME2(struname)(const struct szc_dga_s *szca__, t__ struname *p, struct szc_dgs_s *dst, void ****__dg_ptrs)
#define SZFDECL_STATIC(t__, struname, p, dst)                                                                                       \
  static inline int SZFNAME2(struname)(const struct szc_dga_s *szca__, t__ struname *p, struct szc_dgs_s *dst, void ****__dg_ptrs); \
  static int SZFNAME(struname)(const struct szc_dga_s *szca__, void *p, struct szc_dgs_s *dst) {                                    \
    void ***ptrs = NULL;                                                                                                            \
    int ans = SZFNAME2(struname)(szca__, (t__ struname *)p, dst, &ptrs);                                                            \
    szc_cvector_free(ptrs);                                                                                                         \
    return ans;                                                                                                                     \
  }                                                                                                                                 \
  static inline int SZFNAME2(struname)(const struct szc_dga_s *szca__, t__ struname *p, struct szc_dgs_s *dst, void ****__dg_ptrs)

#define SZFREAD(struname, ll, p, data, datasz) \
  do {                                         \
    struct szc_dgs_s *d = szca_r.szc_init();   \
    szca_r.szc_set_val(d, datasz, data);       \
    if (SZFNAME(struname)(&szca_r, p, d)) {    \
      szca_r.szc_destruct(d);                  \
      goto szcfail;                            \
    }                                          \
    (ll) = szca_r.szc_get_len(d);              \
    szca_r.szc_destruct(d);                    \
  } while (0)
#define SZFWRITE(struname, ll, p, buf, bufsz) \
  do {                                        \
    struct szc_dgs_s *d = szca_w.szc_init();  \
    if (SZFNAME(struname)(&szca_w, p, d)) {   \
      szca_w.szc_destruct(d);                 \
      goto szcfail;                           \
    }                                         \
    (ll) = szca_w.szc_get_val(d, bufsz, buf); \
    szca_w.szc_destruct(d);                   \
  } while (0)

#define szc_member_size(type, member) (sizeof(((type *)0)->member))

#endif  // SERZCORE_UTILS_H
