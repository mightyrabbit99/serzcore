/**
 * Copyright (c) 2025 mightyrabbit99
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `serzcore.c` for details.
 */

#ifndef SERZCORE_H
#define SERZCORE_H

#include <stdint.h>
#include <stdio.h>

#define _target_ex void *

enum szcmode_e {
  szcmode_read,
  szcmode_write,
};

enum szc_dtyp_e { cdef_SZ_o, cdef_SZ_o2, cdef_SZ_o3, cdef_SZ_b, cdef_SZ_b2 };

struct szc_dga_s;
struct szc_dgs_s;
typedef uint32_t szcv_t;

typedef int (*szc_ff_t)(const struct szc_dga_s *, _target_ex, struct szc_dgs_s *);

/////
int szc_get_mode_r(void);
struct szc_dgs_s *szc_init_r(void);
size_t szc_get_len_r(struct szc_dgs_s *d);
void szc_set_maxlen_r(struct szc_dgs_s *d, size_t maxlen);
size_t szc_get_maxlen_r(struct szc_dgs_s *d);
size_t szc_get_val_r(struct szc_dgs_s *d, size_t len, uint8_t *val_ptr);
void szc_set_val_r(struct szc_dgs_s *d, size_t len, uint8_t *val);
void szc_destruct_r(struct szc_dgs_s *d);
void szc_fprint_r(FILE *stream, struct szc_dgs_s *d);
int szcy_r(uint8_t typ, size_t count, szcv_t target, struct szc_dgs_s *d);
int szcyy_r(uint8_t typ, size_t count, uint8_t *target, struct szc_dgs_s *d);
int szcmlc_r(void **target, size_t sz);
int szcrealc_r(void **target, size_t sz);
void *szcmemset_r(uint8_t *s, int c, size_t sz);
void szcfree_r(void *target);
void szcfree2_r(void **target_p);
int szcyf_r(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d);
int szcys_val_r(struct szc_dgs_s *target, struct szc_dgs_s *d);

int szc_get_mode_w(void);
struct szc_dgs_s *szc_init_w(void);
size_t szc_get_len_w(struct szc_dgs_s *d);
void szc_set_maxlen_w(struct szc_dgs_s *d, size_t maxlen);
size_t szc_get_maxlen_w(struct szc_dgs_s *d);
size_t szc_get_val_w(struct szc_dgs_s *d, size_t len, uint8_t *val_ptr);
void szc_set_val_w(struct szc_dgs_s *d, size_t len, uint8_t *val);
void szc_destruct_w(struct szc_dgs_s *d);
void szc_fprint_w(FILE *stream, struct szc_dgs_s *d);
int szcy_w(uint8_t typ, size_t count, szcv_t target, struct szc_dgs_s *d);
int szcyy_w(uint8_t typ, size_t count, uint8_t *target, struct szc_dgs_s *d);
int szcmlc_w(void **target, size_t sz);
int szcrealc_w(void **target, size_t sz);
void *szcmemset_w(uint8_t *s, int c, size_t sz);
void szcfree_w(void *target);
void szcfree2_w(void **target_p);
int szcyf_w(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d);
int szcys_val_w(struct szc_dgs_s *target, struct szc_dgs_s *d);
void szc_set_mem_functions(void *(*malloc_fn)(size_t), void *(*realloc_fn)(void *, size_t), void (*free_fn)(void *));

////

struct szc_dga_s {
  int (*szc_get_mode)(void);
  struct szc_dgs_s *(*szc_init)(void);
  size_t (*szc_get_len)(struct szc_dgs_s *);
  void (*szc_set_maxlen)(struct szc_dgs_s *, size_t);
  size_t (*szc_get_maxlen)(struct szc_dgs_s *);
  size_t (*szc_get_val)(struct szc_dgs_s *, size_t, uint8_t *);
  void (*szc_set_val)(struct szc_dgs_s *, size_t, uint8_t *);
  void (*szc_destruct)(struct szc_dgs_s *);
  void (*szc_fprint)(FILE *, struct szc_dgs_s *);
  int (*szcy)(uint8_t, size_t, szcv_t, struct szc_dgs_s *);
  int (*szcyy)(uint8_t, size_t, uint8_t *, struct szc_dgs_s *);
  int (*szcmlc)(void **, size_t);
  int (*szcrealc)(void **, size_t);
  void *(*szcmemset)(uint8_t *, int, size_t);
  void (*szcfree)(void *);
  void (*szcfree2)(void **);
  int (*szcyf)(szc_ff_t, _target_ex, struct szc_dgs_s *);
  int (*szcys_val)(struct szc_dgs_s *, struct szc_dgs_s *);
};

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
    .szcy = szcy_r,
    .szcyy = szcyy_r,
    .szcmlc = szcmlc_r,
    .szcrealc = szcrealc_r,
    .szcmemset = szcmemset_r,
    .szcfree = szcfree_r,
    .szcfree2 = szcfree2_r,
    .szcyf = szcyf_r,
    .szcys_val = szcys_val_r,
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
    .szcy = szcy_w,
    .szcyy = szcyy_w,
    .szcmlc = szcmlc_w,
    .szcrealc = szcrealc_w,
    .szcmemset = szcmemset_w,
    .szcfree = szcfree_w,
    .szcfree2 = szcfree2_w,
    .szcyf = szcyf_w,
    .szcys_val = szcys_val_w,
};

#endif  // SERZCORE_H
