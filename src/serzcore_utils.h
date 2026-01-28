/**
 * Copyright (c) 2025 Tan Wah Ken
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `serzcore.c` for details.
 */

#ifndef SERZCORE_UTILS_H
#define SERZCORE_UTILS_H
#include "serzcore.h"

#ifndef SZC_DST_NAME
#define SZC_DST_NAME szc_dst__
#endif  // SZC_DST_NAME
#ifndef SZC_SZCA_NAME
#define SZC_SZCA_NAME szca__
#endif  // SZC_SZCA_NAME
#ifndef SZC_DGARR_NAME
#define SZC_DGARR_NAME szc_dg_ptrs__
#endif  // SZC_DGARR_NAME
#ifndef SZF_NAME_PREFIX
#define SZF_NAME_PREFIX __szcf
#endif  // SZF_NAME_PREFIX

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
        func((vec)[szc_cvector_size(vec) - i - 1]); \
      }                                             \
    }                                               \
  } while (0)
#define szc_cvector_begin(vec) (vec)
#define szc_cvector_end(vec) ((vec) ? &((vec)[szc_cvector_size(vec)]) : NULL)
#define szc_cvector_for_each_in(it, vec) for (it = szc_cvector_end(vec) - 1; it >= szc_cvector_begin(vec); it--)

#define SZC_PASTER(x, y) x##_##y
#define SZC_CONCAT(x, y) SZC_PASTER(x, y)

#define _szcy_exec(f1, ...)                                   \
  do {                                                        \
    int ans__;                                                \
    if (ans__ = SZC_SZCA_NAME->f1(__VA_ARGS__)) return ans__; \
  } while (0)
#define szcyf(f, target_ex, d) _szcy_exec(szcyf, f, target_ex, d)
#define szcys_val(target, d) _szcy_exec(szcys_val, target, d)

#define szcy(typ, count, target) _szcy_exec(szcy, typ, count, (uint8_t *)(target), SZC_DST_NAME)
#define szcyy(typ, count, target) _szcy_exec(szcyy, typ, count, (uint8_t *)(target), SZC_DST_NAME)
// #define szcf(struname, p) SZFNAME(struname)(SZC_SZCA_NAME, p, SZC_DST_NAME)
#define szcf(struname, p) _szcy_exec(szcyff, SZFNAME(struname), p, SZC_DST_NAME)

#define szcmlc(target, sz) _szcy_exec(szcmlc, target, sz)
#define szcrealc(target, sz) _szcy_exec(szcrealc, target, sz)
#define szcmemset(s, c, sz) SZC_SZCA_NAME->szcmemset(s, c, sz)
#define szcdelete(pt) SZC_SZCA_NAME->szcfree(pt)
#define szc_get_mode() SZC_SZCA_NAME->szc_get_mode()

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define szcyx(typ, count, bbcnt, target) szcy(typ, count, (target) + ((bbcnt) - szc_count_oct(typ, count)))
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define szcyx(typ, count, bbcnt, target) szcy(typ, count, target)
#else
#error byte order not defined
#endif
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define szcyyx(typ, count, bbcnt, target) szcyy(typ, count, (target) + ((bbcnt) - szc_count_oct(typ, count)))
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define szcyyx(typ, count, bbcnt, target) szcyy(typ, count, target)
#else
#error byte order not defined
#endif
#define szcval(typ, count, valtyp, val)                                                      \
  do {                                                                                       \
    valtyp target__ = val;                                                                   \
    unsigned long long int count2__ = count;                                                 \
    if (szc_conv_1(typ, sizeof(valtyp)) < count) count2__ = szc_conv_1(typ, sizeof(valtyp)); \
    szcyx(typ, count2__, szc_conv_1(typ, sizeof(valtyp)), &target__);                        \
  } while (0)
#define szcmlcl(ptr, len)                                   \
  do {                                                      \
    szcmlc((void **)(ptr), len);                            \
    szc_cvector_push_back(*SZC_DGARR_NAME, (void **)(ptr)); \
  } while (0)
#define szcrealcl(ptr, len)                                                         \
  do {                                                                              \
    void *ptr_orig__ = *(ptr);                                                      \
    szcrealc((void **)(ptr), len);                                                  \
    if (ptr_orig__ == NULL) szc_cvector_push_back(*SZC_DGARR_NAME, (void **)(ptr)); \
  } while (0)
#define szcmlcyy(typ, len, ptr)        \
  do {                                 \
    szcmlcl(&(ptr), len);              \
    szcyy(typ, len, (uint8_t *)(ptr)); \
  } while (0)
#define szclvp(typ, len, maxlen, ptr)                                                                     \
  do {                                                                                                    \
    if (len > maxlen) return 1;                                                                           \
    szcyyx(typ, szc_conv_1(typ, szc_get_ctnsz(maxlen)), szc_conv_1(typ, sizeof(len)), (uint8_t *)&(len)); \
    szcmlcyy(typ, szc_conv_1(typ, (len) * sizeof(*(ptr))), ptr);                                          \
  } while (0)
#define szclvstr(typ, maxlen, ptr)                                                               \
  do {                                                                                           \
    size_t len__ = (ptr) == NULL ? 0 : (strnlen(ptr, maxlen) + 1);                               \
    if (len__ > maxlen) return 1;                                                                \
    szcyyx(typ, szc_conv_1(typ, szc_get_ctnsz(maxlen)), szc_conv_1(typ, sizeof(len__)), &len__); \
    szcmlcyy(typ, szc_conv_1(typ, len__), ptr);                                                  \
  } while (0)
#define szclvrcrse(typ, tlv_len_t, ff, target)            \
  do {                                                    \
    struct szc_dgs_s *d2__ = SZC_SZCA_NAME->szc_init();   \
    if (d2__ == NULL) return 1;                           \
    szcyf(ff, target, d2__);                              \
    tlv_len_t len__ = SZC_SZCA_NAME->szc_get_len(d2__);   \
    szcyy(typ, sizeof(len__), &len__);                    \
    SZC_SZCA_NAME->szc_set_maxlen(d2__, len__);           \
    size_t l1 = SZC_SZCA_NAME->szc_get_len(SZC_DST_NAME); \
    szcys_val(d2__, SZC_DST_NAME);                        \
    size_t l2 = SZC_SZCA_NAME->szc_get_len(SZC_DST_NAME); \
    SZC_SZCA_NAME->szc_destruct(d2__);                    \
    d2__ = NULL;                                          \
    if (l2 - l1 != len__) return 1;                       \
  } while (0)

#define szc_set_arr_i_ex(i) SZC_SZCA_NAME->szc_set_arr_i_ex(SZC_DST_NAME, i)
#define szcy_ex(typ, count, target, name, ...) _szcy_exec(szcy_ex, typ, count, (uint8_t *)(target), SZC_DST_NAME, name, __VA_ARGS__)
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define szcyx_ex(typ, count, bbcnt, target, name, ...) szcy_ex(typ, count, (target) + ((bbcnt) - szc_count_oct(typ, count)), name, __VA_ARGS__)
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define szcyx_ex(typ, count, bbcnt, target, name, ...) szcy_ex(typ, count, target, name, __VA_ARGS__)
#else
#error byte order not defined
#endif
#define szc_get_fieldlen_ex(typ, count, target, maxlen, name, ...) _szcy_exec(szc_get_fieldlen_ex, typ, count, (uint8_t *)(target), maxlen, SZC_DST_NAME, name, __VA_ARGS__)
#define szcval_ex(typ, count, valtyp, val, name, ...)                                        \
  do {                                                                                       \
    valtyp target__ = val;                                                                   \
    unsigned long long int count2__ = count;                                                 \
    if (szc_conv_1(typ, sizeof(valtyp)) < count) count2__ = szc_conv_1(typ, sizeof(valtyp)); \
    szcyx_ex(typ, count2__, szc_conv_1(typ, sizeof(valtyp)), &target__, name, __VA_ARGS__);  \
  } while (0)
#define szcyy_ex(typ, count, target, name, ...) _szcy_exec(szcyy_ex, typ, count, (uint8_t *)(target), SZC_DST_NAME, name, __VA_ARGS__)
#define szcf_ex(struname, p, name) _szcy_exec(szcyff_ex, SZFNAME(struname), p, SZC_DST_NAME, name)
#define szcmlcyy_ex(typ, len, ptr, name, ...)                \
  do {                                                       \
    szcmlcl(&(ptr), len);                                    \
    szcyy_ex(typ, len, (uint8_t *)(ptr), name, __VA_ARGS__); \
  } while (0)
#define szclvstr_ex(typ, maxlen, ptr, name)                                                                        \
  do {                                                                                                             \
    size_t len__;                                                                                                  \
    szc_get_fieldlen_ex(typ, szc_conv_1(typ, sizeof(len__)), (uint8_t *)&(len__), maxlen, name, szc_extyp_string); \
    szcyyx(typ, szc_get_ctnsz(maxlen), sizeof(len__), &len__);                                                     \
    szcmlcyy_ex(typ, len__, ptr, name, szc_extyp_string);                                                          \
  } while (0)
#define szclvp_ex(typ, len, maxlen, ptr, name, ...)                                                       \
  do {                                                                                                    \
    szc_get_fieldlen_ex(typ, szc_conv_1(typ, sizeof(len)), (uint8_t *)&(len), maxlen, name, __VA_ARGS__); \
    szcyyx(typ, szc_conv_1(typ, szc_get_ctnsz(maxlen)), szc_conv_1(typ, sizeof(len)), (uint8_t *)&(len)); \
    szcmlcyy_ex(typ, szc_conv_1(typ, (len) * sizeof(*(ptr))), ptr, name, __VA_ARGS__);                    \
  } while (0)

#define SZF_NAME_PREFIX2 SZC_CONCAT(SZF_NAME_PREFIX, inner)
#define SZFNAME(struname) SZC_CONCAT(SZF_NAME_PREFIX, struname)
#define SZFNAME2(struname) SZC_CONCAT(SZF_NAME_PREFIX2, struname)
#define SZFDECL2(t__, struname, p, dst) int SZFNAME(struname)(const struct szc_dga_s *szca, void *p, struct szc_dgs_s *dst)
#define SZFDECL(t__, struname, p)                                                                                                                        \
  static inline int SZFNAME2(struname)(const struct szc_dga_s *SZC_SZCA_NAME, t__ struname *p, struct szc_dgs_s *SZC_DST_NAME, void ****SZC_DGARR_NAME); \
  SZFDECL2(t__, struname, p, dst) {                                                                                                                      \
    void ***ptrs = NULL;                                                                                                                                 \
    int ans = SZFNAME2(struname)(szca, (t__ struname *)p, dst, &ptrs);                                                                                   \
    if (ans != 0)                                                                                                                                        \
      szc_cvector_for_each(ptrs, szca->szcfree2);                                                                                                        \
    else                                                                                                                                                 \
      szc_cvector_for_each(ptrs, szca->szc_ptop_ex);                                                                                                     \
    szc_cvector_free(ptrs);                                                                                                                              \
    return ans;                                                                                                                                          \
  }                                                                                                                                                      \
  static inline int SZFNAME2(struname)(const struct szc_dga_s *SZC_SZCA_NAME, t__ struname *p, struct szc_dgs_s *SZC_DST_NAME, void ****SZC_DGARR_NAME)
#define SZFDECL_STATIC(t__, struname, p)                                                                                                                 \
  static inline int SZFNAME2(struname)(const struct szc_dga_s *SZC_SZCA_NAME, t__ struname *p, struct szc_dgs_s *SZC_DST_NAME, void ****SZC_DGARR_NAME); \
  static SZFDECL2(t__, struname, p, dst) {                                                                                                               \
    void ***ptrs = NULL;                                                                                                                                 \
    int ans = SZFNAME2(struname)(szca, (t__ struname *)p, dst, &ptrs);                                                                                   \
    if (ans != 0)                                                                                                                                        \
      szc_cvector_for_each(ptrs, szca->szcfree2);                                                                                                        \
    else                                                                                                                                                 \
      szc_cvector_for_each(ptrs, szca->szc_ptop_ex);                                                                                                     \
    szc_cvector_free(ptrs);                                                                                                                              \
    return ans;                                                                                                                                          \
  }                                                                                                                                                      \
  static inline int SZFNAME2(struname)(const struct szc_dga_s *SZC_SZCA_NAME, t__ struname *p, struct szc_dgs_s *SZC_DST_NAME, void ****SZC_DGARR_NAME)

#if defined(SERZCORE_LUA)
#define SZFREAD2(t__, struname, ll, data, datasz, ctx1) \
  do {                                                  \
    t__ struname p__ = (t__ struname){0};               \
    struct szc_dgs_s *d__ = szca_r.szc_init();          \
    if (d__ == NULL) return -1;                         \
    szca_r.szc_set_ctx_ex(d__, ctx1);                   \
    szca_r.szc_set_val(d__, datasz, data);              \
    if (SZFNAME(struname)(&szca_r, &p__, d__)) {        \
      szca_r.szc_destruct(d__);                         \
      return -1;                                        \
    }                                                   \
    (ll) = szca_r.szc_get_len(d__);                     \
    szca_r.szc_destruct(d__);                           \
  } while (0)
#define SZFWRITE2(t__, struname, ll, buf, bufsz, ctx1) \
  do {                                                 \
    t__ struname p__ = (t__ struname){0};              \
    struct szc_dgs_s *d__ = szca_w.szc_init();         \
    if (d__ == NULL) return -1;                        \
    szca_w.szc_set_ctx_ex(d__, ctx1);                  \
    if (SZFNAME(struname)(&szca_w, &p__, d__)) {       \
      szca_w.szc_destruct(d__);                        \
      return -1;                                       \
    }                                                  \
    (ll) = szca_w.szc_get_val(d__, bufsz, buf);        \
    szca_w.szc_destruct(d__);                          \
  } while (0)
#else
#define SZFREAD(struname, ll, p, data, datasz) \
  do {                                         \
    struct szc_dgs_s *d__ = szca_r.szc_init(); \
    if (d__ == NULL) return -1;                \
    szca_r.szc_set_val(d__, datasz, data);     \
    if (SZFNAME(struname)(&szca_r, p, d__)) {  \
      szca_r.szc_destruct(d__);                \
      return -1;                               \
    }                                          \
    (ll) = szca_r.szc_get_len(d__);            \
    szca_r.szc_destruct(d__);                  \
  } while (0)
#define SZFWRITE(struname, ll, p, buf, bufsz)   \
  do {                                          \
    struct szc_dgs_s *d__ = szca_w.szc_init();  \
    if (d__ == NULL) return -1;                 \
    if (SZFNAME(struname)(&szca_w, p, d__)) {   \
      szca_w.szc_destruct(d__);                 \
      return -1;                                \
    }                                           \
    (ll) = szca_w.szc_get_val(d__, bufsz, buf); \
    szca_w.szc_destruct(d__);                   \
  } while (0)
#endif
#define SZFOUTEXEC(t__, struname, f, ctx1)       \
  do {                                           \
    const uint8_t *buf;                          \
    size_t ll;                                   \
    t__ struname p__ = (t__ struname){0};        \
    struct szc_dgs_s *d__ = szca_w.szc_init();   \
    if (d__ == NULL) return -1;                  \
    szca_w.szc_set_ctx_ex(d__, ctx1);            \
    if (SZFNAME(struname)(&szca_w, &p__, d__)) { \
      szca_w.szc_destruct(d__);                  \
      return -1;                                 \
    }                                            \
    buf = szca_w.szc_retrieve_val(d__, &(ll));   \
    f(ctx1, buf, ll);                            \
    szca_w.szc_destruct(d__);                    \
  } while (0)

#define szc_member_size(type, member) (sizeof(((type *)0)->member))

#endif  // SERZCORE_UTILS_H
