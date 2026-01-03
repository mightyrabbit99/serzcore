/*
 * Copyright (c) 2025 mightyrabbit99
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

struct szc_dgsr_s {
  szc_ff_t f;
  struct szc_dga_s *dga1;
  _target_ex target_ex;
  long long int bitlen;
  size_t maxlen;
  uint8_t *val;
};

int szc_get_mode_r(void) { return szcmode_read; }

struct szc_dgs_s *szc_init_r(void) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)szc_malloc(sizeof(struct szc_dgsr_s));
  dd->bitlen = 0;
  dd->maxlen = 0;
  dd->dga1 = &szca_r;
  return (struct szc_dgs_s *)dd;
}

void szc_fprint_r(FILE *stream, struct szc_dgs_s *d) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  int i;
  for (i = 0; i < dd->bitlen >> 3; i++) {
    fprintf(stream, "%.2x", dd->val[i]);
  }
}

size_t szc_get_len_r(struct szc_dgs_s *d) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  return (size_t)dd->bitlen >> 3;
}

void szc_set_maxlen_r(struct szc_dgs_s *d, size_t maxlen) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  dd->maxlen = maxlen;
}

size_t szc_get_maxlen_r(struct szc_dgs_s *d) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  return dd->maxlen;
}

size_t szc_get_val_r(struct szc_dgs_s *d, size_t len, uint8_t *val_ptr) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  size_t ll = MIN(szc_get_len_r(d), len);
  memcpy(val_ptr, dd->val, ll);
  return ll;
}

void szc_set_val_r(struct szc_dgs_s *d, size_t len, uint8_t *val) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  dd->maxlen = len;
  dd->val = val;
}

void szc_destruct_r(struct szc_dgs_s *d) { szc_free(d); }

static inline void _szcpy(uint8_t typ, uint8_t *dst, uint8_t *src, size_t count, uint8_t pos_bb);

static inline int szcyy_b_r(size_t count, uint8_t *target, struct szc_dgs_s *d) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  size_t start = dd->bitlen >> 3;
  size_t end = start + (count >> 3) + (count % 8 == 0 ? 0 : 1);
  if (end > dd->maxlen) return 1;

  _szcpy(cdef_SZ_b, target, dd->val + start, count, dd->bitlen % 8);
  dd->bitlen += count;
  return 0;
}

static inline int szcyy_b2_r(size_t count, uint8_t *target, struct szc_dgs_s *d) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  size_t start = dd->bitlen >> 3;
  size_t end = start + (count >> 3) + (count % 8 == 0 ? 0 : 1);
  if (end > dd->maxlen) return 1;

  _szcpy(cdef_SZ_b2, target, dd->val + start, count, dd->bitlen % 8);
  dd->bitlen += count;
  return 0;
}

static inline int szcyy_o_r(size_t count, uint8_t *target, struct szc_dgs_s *d) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  dd->bitlen += dd->bitlen % 8 == 0 ? 0 : (8 - (dd->bitlen % 8));
  size_t start = dd->bitlen >> 3;
  size_t end = start + count;
  if (end > dd->maxlen) return 1;

  _szcpy(cdef_SZ_o, target, dd->val + start, count, 0);
  dd->bitlen += count << 3;
  return 0;
}

static inline int szcyy_o2_r(size_t count, uint8_t *target, struct szc_dgs_s *d) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  dd->bitlen += dd->bitlen % 8 == 0 ? 0 : (8 - (dd->bitlen % 8));
  size_t start = dd->bitlen >> 3;
  size_t end = start + count;
  if (end > dd->maxlen) return 1;

  _szcpy(cdef_SZ_o, target, dd->val + start, count, 0);
  dd->bitlen += count << 3;
  return 0;
}

int szcy_r(uint8_t typ, size_t count, szcv_t target, struct szc_dgs_s *d) {
  if (count == 0) return 1;
  if (count > sizeof(szcv_t)) return 1;
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  switch (typ) {
    case cdef_SZ_o:
    case cdef_SZ_o2:
      if (((dd->bitlen >> 3) + (dd->bitlen % 8 == 0 ? 0 : 1) + count) > dd->maxlen) return 1;
      dd->bitlen += dd->bitlen % 8 == 0 ? 0 : (8 - (dd->bitlen % 8));
      dd->bitlen += count << 3;
      break;
    case cdef_SZ_b:
    case cdef_SZ_b2:
      if (((dd->bitlen + count) >> 3) > dd->maxlen) return 1;
      dd->bitlen += count;
      break;
    default:
      return 1;
  }
  return 0;
}

int szcyy_r(uint8_t typ, size_t count, uint8_t *target, struct szc_dgs_s *d) {
  if (count == 0) return 0;
  int i;
  switch (typ) {
    case cdef_SZ_o:
      return szcyy_o_r(count, target, d);
    case cdef_SZ_o2:
      return szcyy_o2_r(count, target, d);
    case cdef_SZ_b:
      return szcyy_b_r(count, target, d);
    case cdef_SZ_b2:
      return szcyy_b2_r(count, target, d);
  }
  return 0;
}

int szcmlc_r(void **target, size_t sz) {
  if (sz == 0) {
    *target = NULL;
    return 0;
  }
  *target = (void *)szc_malloc(sz);
  if (*target == NULL) return 1;
  memset(*target, 0, sz);
  return 0;
}

int szcrealc_r(void **target, size_t sz) {
  if (sz == 0) {
    *target = NULL;
    return 0;
  }
  *target = (void *)szc_realloc(*target, sz);
  if (*target == NULL) return 1;
  return 0;
}

void *szcmemset_r(uint8_t *s, int c, size_t sz) {
  return memset(s, c, sz);
}

void szcfree_r(void *target) {
  if (target) szc_free(target);
}

void szcfree2_r(void **target_p) {
  if (*target_p) szc_free(*target_p);
  *target_p = NULL;
}

int szcyf_r(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  dd->f = f;
  dd->target_ex = target_ex;
  return 0;
}

int szcys_val_r(struct szc_dgs_s *target, struct szc_dgs_s *d) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  struct szc_dgsr_s *dd_t = (struct szc_dgsr_s *)target;
  if (dd_t->maxlen > 0) {
    if (dd_t->maxlen > dd->maxlen - (dd->bitlen >> 3)) return 1;
    szc_set_val_r(target, dd_t->maxlen, &dd->val[dd->bitlen >> 3]);
    if (dd_t->f(dd_t->dga1, dd_t->target_ex, target)) return 1;
    dd->bitlen += dd_t->bitlen;
  } else {
    if (dd_t->f(dd_t->dga1, dd_t->target_ex, d)) return 1;
  }
  return 0;
}
