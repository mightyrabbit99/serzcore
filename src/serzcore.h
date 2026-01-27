/**
 * Copyright (c) 2025 Tan Wah Ken
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `serzcore.c` for details.
 */

#ifndef SERZCORE_H
#define SERZCORE_H

#include <stdint.h>
#include <stdio.h>

#define _target_ex void *

typedef enum {
  szcmode_read,
  szcmode_write,
} szcmode_t;

typedef enum {
  szc_dtyp_o,
  szc_dtyp_o2,
  szc_dtyp_o3,
  szc_dtyp_b,
  szc_dtyp_b2,
  _szc_dtyp_max
} szc_dtyp_t;

typedef enum {
  szc_extyp_bool,
  szc_extyp_int,
  szc_extyp_double,
  szc_extyp_string,
  szc_extyp_data,
  _szc_extyp_max,
} szc_extyp_t;

static inline int szc_typ_is_octal(szc_dtyp_t typ) {
  switch (typ) {
    case szc_dtyp_o:
    case szc_dtyp_o2:
    case szc_dtyp_o3:
      return 1;
    case szc_dtyp_b:
    case szc_dtyp_b2:
      return 0;
    default:
      return -1;
  }
}

#define szc_count_oct(typ, count) ((size_t)(szc_typ_is_octal(typ) ? (count) : ((count) >> 3) + ((count) % 8 == 0 ? 0 : 1)))
#define szc_count_bit(typ, count) ((unsigned long long int)(szc_typ_is_octal(typ) ? ((unsigned long long int)(count) << 3) : (count)))
#define szc_conv_1(typ, count) ((unsigned long long int)(szc_typ_is_octal(typ) ? (count) : ((unsigned long long int)(count) << 3)))

struct szc_dga_s;
struct szc_dgs_s;

typedef int (*szc_ff_t)(const struct szc_dga_s *, _target_ex, struct szc_dgs_s *);

/////
#if defined(SERZCORE_LUA)
szcmode_t szc_get_mode_r_lua(void);
struct szc_dgs_s *szc_init_r_lua(void);
size_t szc_get_len_r_lua(struct szc_dgs_s *d);
void szc_set_maxlen_r_lua(struct szc_dgs_s *d, size_t maxlen);
size_t szc_get_maxlen_r_lua(struct szc_dgs_s *d);
size_t szc_get_val_r_lua(struct szc_dgs_s *d, size_t len, uint8_t *val_ptr);
void szc_set_val_r_lua(struct szc_dgs_s *d, size_t len, uint8_t *val);
void szc_destruct_r_lua(struct szc_dgs_s *d);
void szc_fprint_r_lua(FILE *stream, struct szc_dgs_s *d);
void szc_set_ctx_r_ex_lua(struct szc_dgs_s *d, void *ctx1);
int szc_get_fieldlen_r_ex_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, szc_extyp_t extyp, const char *name);
int szcy_r_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d);
int szcyy_r_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d);
int szcy_r_ex_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, szc_extyp_t extyp, const char *name);
int szcyy_r_ex_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, szc_extyp_t extyp, const char *name);
int szcmlc_r_lua(void **target, size_t sz);
int szcrealc_r_lua(void **target, size_t sz);
void *szcmemset_r_lua(uint8_t *s, int c, size_t sz);
void szcfree_r_lua(void *target);
void szcfree2_r_lua(void **target_p);
void szc_ptop_r_ex_lua(void **target_p);
int szcyf_r_lua(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d);
int szcys_val_r_lua(struct szc_dgs_s *target, struct szc_dgs_s *d);
int szcyff_r_lua(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d);
int szcyff_r_ex_lua(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d, const char *name);

szcmode_t szc_get_mode_w_lua(void);
struct szc_dgs_s *szc_init_w_lua(void);
size_t szc_get_len_w_lua(struct szc_dgs_s *d);
void szc_set_maxlen_w_lua(struct szc_dgs_s *d, size_t maxlen);
size_t szc_get_maxlen_w_lua(struct szc_dgs_s *d);
size_t szc_get_val_w_lua(struct szc_dgs_s *d, size_t len, uint8_t *val_ptr);
void szc_set_val_w_lua(struct szc_dgs_s *d, size_t len, uint8_t *val);
void szc_destruct_w_lua(struct szc_dgs_s *d);
void szc_fprint_w_lua(FILE *stream, struct szc_dgs_s *d);
void szc_set_ctx_w_ex_lua(struct szc_dgs_s *d, void *ctx1);
int szc_get_fieldlen_w_ex_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, szc_extyp_t extyp, const char *name);
int szcy_w_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d);
int szcyy_w_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d);
int szcy_w_ex_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, szc_extyp_t extyp, const char *name);
int szcyy_w_ex_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, szc_extyp_t extyp, const char *name);
int szcmlc_w_lua(void **target, size_t sz);
int szcrealc_w_lua(void **target, size_t sz);
void *szcmemset_w_lua(uint8_t *s, int c, size_t sz);
void szcfree_w_lua(void *target);
void szcfree2_w_lua(void **target_p);
void szc_ptop_w_ex_lua(void **target_p);
int szcyf_w_lua(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d);
int szcys_val_w_lua(struct szc_dgs_s *target, struct szc_dgs_s *d);
int szcyff_w_lua(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d);
int szcyff_w_ex_lua(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d, const char *name);

#else

szcmode_t szc_get_mode_r(void);
struct szc_dgs_s *szc_init_r(void);
size_t szc_get_len_r(struct szc_dgs_s *d);
void szc_set_maxlen_r(struct szc_dgs_s *d, size_t maxlen);
size_t szc_get_maxlen_r(struct szc_dgs_s *d);
size_t szc_get_val_r(struct szc_dgs_s *d, size_t len, uint8_t *val_ptr);
void szc_set_val_r(struct szc_dgs_s *d, size_t len, uint8_t *val);
void szc_destruct_r(struct szc_dgs_s *d);
void szc_fprint_r(FILE *stream, struct szc_dgs_s *d);
void szc_set_ctx_r_ex(struct szc_dgs_s *d, void *ctx1);
int szc_get_fieldlen_r_ex(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, szc_extyp_t extyp, const char *name);
int szcy_r(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d);
int szcyy_r(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d);
int szcy_r_ex(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, szc_extyp_t extyp, const char *name);
int szcyy_r_ex(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, szc_extyp_t extyp, const char *name);
int szcmlc_r(void **target, size_t sz);
int szcrealc_r(void **target, size_t sz);
void *szcmemset_r(uint8_t *s, int c, size_t sz);
void szcfree_r(void *target);
void szcfree2_r(void **target_p);
void szc_ptop_r_ex(void **target_p);
int szcyf_r(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d);
int szcys_val_r(struct szc_dgs_s *target, struct szc_dgs_s *d);
int szcyff_r(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d);
int szcyff_r_ex(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d, const char *name);

szcmode_t szc_get_mode_w(void);
struct szc_dgs_s *szc_init_w(void);
size_t szc_get_len_w(struct szc_dgs_s *d);
void szc_set_maxlen_w(struct szc_dgs_s *d, size_t maxlen);
size_t szc_get_maxlen_w(struct szc_dgs_s *d);
size_t szc_get_val_w(struct szc_dgs_s *d, size_t len, uint8_t *val_ptr);
void szc_set_val_w(struct szc_dgs_s *d, size_t len, uint8_t *val);
void szc_destruct_w(struct szc_dgs_s *d);
void szc_fprint_w(FILE *stream, struct szc_dgs_s *d);
void szc_set_ctx_w_ex(struct szc_dgs_s *d, void *ctx1);
int szc_get_fieldlen_w_ex(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, szc_extyp_t extyp, const char *name);
int szcy_w(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d);
int szcyy_w(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d);
int szcy_w_ex(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, szc_extyp_t extyp, const char *name);
int szcyy_w_ex(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, szc_extyp_t extyp, const char *name);
int szcmlc_w(void **target, size_t sz);
int szcrealc_w(void **target, size_t sz);
void *szcmemset_w(uint8_t *s, int c, size_t sz);
void szcfree_w(void *target);
void szcfree2_w(void **target_p);
void szc_ptop_w_ex(void **target_p);
int szcyf_w(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d);
int szcys_val_w(struct szc_dgs_s *target, struct szc_dgs_s *d);
int szcyff_w(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d);
int szcyff_w_ex(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d, const char *name);

#endif // SERZCORE_LUA

void szc_set_mem_functions(void *(*malloc_fn)(size_t), void *(*realloc_fn)(void *, size_t), void (*free_fn)(void *));

////

struct szc_dga_s {
  szcmode_t (*szc_get_mode)(void);
  struct szc_dgs_s *(*szc_init)(void);
  size_t (*szc_get_len)(struct szc_dgs_s *);
  void (*szc_set_maxlen)(struct szc_dgs_s *, size_t);
  size_t (*szc_get_maxlen)(struct szc_dgs_s *);
  size_t (*szc_get_val)(struct szc_dgs_s *, size_t, uint8_t *);
  void (*szc_set_val)(struct szc_dgs_s *, size_t, uint8_t *);
  void (*szc_destruct)(struct szc_dgs_s *);
  void (*szc_fprint)(FILE *, struct szc_dgs_s *);
  void (*szc_set_ctx_ex)(struct szc_dgs_s *, void *);
  int (*szc_get_fieldlen_ex)(szc_dtyp_t, unsigned long long int, uint8_t *, struct szc_dgs_s *, szc_extyp_t, const char *);
  int (*szcy)(szc_dtyp_t, unsigned long long int, uint8_t *, struct szc_dgs_s *);
  int (*szcyy)(szc_dtyp_t, unsigned long long int, uint8_t *, struct szc_dgs_s *);
  int (*szcy_ex)(szc_dtyp_t, unsigned long long int, uint8_t *, struct szc_dgs_s *, szc_extyp_t, const char *);
  int (*szcyy_ex)(szc_dtyp_t, unsigned long long int, uint8_t *, struct szc_dgs_s *, szc_extyp_t, const char *);
  int (*szcmlc)(void **, size_t);
  int (*szcrealc)(void **, size_t);
  void *(*szcmemset)(uint8_t *, int, size_t);
  void (*szcfree)(void *);
  void (*szcfree2)(void **);
  void (*szc_ptop_ex)(void **);
  int (*szcyf)(szc_ff_t, _target_ex, struct szc_dgs_s *);
  int (*szcys_val)(struct szc_dgs_s *, struct szc_dgs_s *);
  int (*szcyff)(szc_ff_t, _target_ex, struct szc_dgs_s *);
  int (*szcyff_ex)(szc_ff_t, _target_ex, struct szc_dgs_s *, const char *);
};

#if defined(SERZCORE_LUA)
static struct szc_dga_s szca_r = (struct szc_dga_s){
    .szc_get_mode = szc_get_mode_r_lua,
    .szc_init = szc_init_r_lua,
    .szc_get_len = szc_get_len_r_lua,
    .szc_set_maxlen = szc_set_maxlen_r_lua,
    .szc_get_maxlen = szc_get_maxlen_r_lua,
    .szc_get_val = szc_get_val_r_lua,
    .szc_set_val = szc_set_val_r_lua,
    .szc_destruct = szc_destruct_r_lua,
    .szc_fprint = szc_fprint_r_lua,
    .szc_set_ctx_ex = szc_set_ctx_r_ex_lua,
    .szc_get_fieldlen_ex = szc_get_fieldlen_r_ex_lua,
    .szcy = szcy_r_lua,
    .szcyy = szcyy_r_lua,
    .szcy_ex = szcy_r_ex_lua,
    .szcyy_ex = szcyy_r_ex_lua,
    .szcmlc = szcmlc_r_lua,
    .szcrealc = szcrealc_r_lua,
    .szcmemset = szcmemset_r_lua,
    .szcfree = szcfree_r_lua,
    .szcfree2 = szcfree2_r_lua,
    .szc_ptop_ex = szc_ptop_r_ex_lua,
    .szcyf = szcyf_r_lua,
    .szcys_val = szcys_val_r_lua,
    .szcyff = szcyff_r_lua,
    .szcyff_ex = szcyff_r_ex_lua,
};

static struct szc_dga_s szca_w = (struct szc_dga_s){
    .szc_get_mode = szc_get_mode_w_lua,
    .szc_init = szc_init_w_lua,
    .szc_get_len = szc_get_len_w_lua,
    .szc_set_maxlen = szc_set_maxlen_w_lua,
    .szc_get_maxlen = szc_get_maxlen_w_lua,
    .szc_get_val = szc_get_val_w_lua,
    .szc_set_val = szc_set_val_w_lua,
    .szc_destruct = szc_destruct_w_lua,
    .szc_fprint = szc_fprint_w_lua,
    .szc_set_ctx_ex = szc_set_ctx_w_ex_lua,
    .szc_get_fieldlen_ex = szc_get_fieldlen_w_ex_lua,
    .szcy = szcy_w_lua,
    .szcyy = szcyy_w_lua,
    .szcy_ex = szcy_w_ex_lua,
    .szcyy_ex = szcyy_w_ex_lua,
    .szcmlc = szcmlc_w_lua,
    .szcrealc = szcrealc_w_lua,
    .szcmemset = szcmemset_w_lua,
    .szcfree = szcfree_w_lua,
    .szcfree2 = szcfree2_w_lua,
    .szc_ptop_ex = szc_ptop_w_ex_lua,
    .szcyf = szcyf_w_lua,
    .szcys_val = szcys_val_w_lua,
    .szcyff = szcyff_w_lua,
    .szcyff_ex = szcyff_w_ex_lua,
};
#else

static struct szc_dga_s szca_r = (struct szc_dga_s){
    .szc_get_mode = szc_get_mode_r,
    .szc_init = szc_init_r,
    .szc_get_len = szc_get_len_r,
    .szc_set_maxlen = szc_set_maxlen_r,
    .szc_get_maxlen = szc_get_maxlen_r,
    .szc_get_val = szc_get_val_r,
    .szc_set_val = szc_set_val_r,
    .szc_destruct = szc_destruct_r,
    .szc_fprint = szc_fprint_r,
    .szc_set_ctx_ex = szc_set_ctx_r_ex,
    .szc_get_fieldlen_ex = szc_get_fieldlen_r_ex,
    .szcy = szcy_r,
    .szcyy = szcyy_r,
    .szcy_ex = szcy_r_ex,
    .szcyy_ex = szcyy_r_ex,
    .szcmlc = szcmlc_r,
    .szcrealc = szcrealc_r,
    .szcmemset = szcmemset_r,
    .szcfree = szcfree_r,
    .szcfree2 = szcfree2_r,
    .szc_ptop_ex = szc_ptop_r_ex,
    .szcyf = szcyf_r,
    .szcys_val = szcys_val_r,
    .szcyff = szcyff_r,
    .szcyff_ex = szcyff_r_ex,
};

static struct szc_dga_s szca_w = (struct szc_dga_s){
    .szc_get_mode = szc_get_mode_w,
    .szc_init = szc_init_w,
    .szc_get_len = szc_get_len_w,
    .szc_set_maxlen = szc_set_maxlen_w,
    .szc_get_maxlen = szc_get_maxlen_w,
    .szc_get_val = szc_get_val_w,
    .szc_set_val = szc_set_val_w,
    .szc_destruct = szc_destruct_w,
    .szc_fprint = szc_fprint_w,
    .szc_set_ctx_ex = szc_set_ctx_w_ex,
    .szc_get_fieldlen_ex = szc_get_fieldlen_w_ex,
    .szcy = szcy_w,
    .szcyy = szcyy_w,
    .szcy_ex = szcy_w_ex,
    .szcyy_ex = szcyy_w_ex,
    .szcmlc = szcmlc_w,
    .szcrealc = szcrealc_w,
    .szcmemset = szcmemset_w,
    .szcfree = szcfree_w,
    .szcfree2 = szcfree2_w,
    .szc_ptop_ex = szc_ptop_w_ex,
    .szcyf = szcyf_w,
    .szcys_val = szcys_val_w,
    .szcyff = szcyff_w,
    .szcyff_ex = szcyff_w_ex,
};

#endif

#endif  // SERZCORE_H
