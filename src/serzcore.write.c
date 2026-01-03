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

struct szc_dgsw_s {
  struct szc_dga_s *dga1;
  long long int bitlen;
  size_t maxlen;
  uint8_t *val;
};

int szc_get_mode_w(void) { return szcmode_write; }

struct szc_dgs_s *szc_init_w(void) {
  struct szc_dgsw_s *dd = (struct szc_dgsw_s *)szc_malloc(sizeof(struct szc_dgsw_s));
  dd->bitlen = 0;
  dd->maxlen = MAZ_SZ;
  dd->val = (uint8_t *)szc_malloc(1);
  dd->dga1 = &szca_w;
  return (struct szc_dgs_s *)dd;
}

void szc_fprint_w(FILE *stream, struct szc_dgs_s *d) {
  struct szc_dgsw_s *dd = (struct szc_dgsw_s *)d;
  int i;
  for (i = 0; i < dd->bitlen >> 3; i++) {
    fprintf(stream, "%.2x", dd->val[i]);
  }
}

size_t szc_get_len_w(struct szc_dgs_s *d) {
  struct szc_dgsw_s *dd = (struct szc_dgsw_s *)d;
  return (size_t)dd->bitlen >> 3;
}

void szc_set_maxlen_w(struct szc_dgs_s *d, size_t maxlen) {
  struct szc_dgsw_s *dd = (struct szc_dgsw_s *)d;
  dd->maxlen = maxlen;
}

size_t szc_get_maxlen_w(struct szc_dgs_s *d) {
  struct szc_dgsw_s *dd = (struct szc_dgsw_s *)d;
  return dd->maxlen;
}

size_t szc_get_val_w(struct szc_dgs_s *d, size_t len, uint8_t *val_ptr) {
  struct szc_dgsw_s *dd = (struct szc_dgsw_s *)d;
  size_t ll = MIN(szc_get_len_w(d), len);
  memcpy(val_ptr, dd->val, ll);
  return ll;
}

void szc_set_val_w(struct szc_dgs_s *d, size_t len, uint8_t *val) {
  struct szc_dgsw_s *dd = (struct szc_dgsw_s *)d;
  dd->bitlen = len << 3;
  uint8_t *val2 = (uint8_t *)szc_malloc(sizeof(uint8_t) * len);
  memcpy(val2, val, len);
  dd->val = val2;
}

void szc_destruct_w(struct szc_dgs_s *d) {
  struct szc_dgsw_s *dd = (struct szc_dgsw_s *)d;
  if (dd->val) szc_free(dd->val);
  dd->val = NULL;
  szc_free(dd);
}

static inline void _szcpy(uint8_t typ, uint8_t *dst, uint8_t *src, size_t count, uint8_t pos_bb);

static inline int szcyy_b_w(size_t count, uint8_t *target, struct szc_dgs_s *d) {
  struct szc_dgsw_s *dd = (struct szc_dgsw_s *)d;
  size_t start = dd->bitlen >> 3;
  size_t end = ((dd->bitlen + count) >> 3) + ((dd->bitlen + count) % 8 == 0 ? 0 : 1);
  if (end > dd->maxlen) return 1;

  dd->val = szc_realloc(dd->val, end);
  _szcpy(cdef_SZ_b, dd->val + start, target, count, dd->bitlen % 8);
  dd->bitlen += count;
  return 0;
}

static inline int szcyy_b2_w(size_t count, uint8_t *target, struct szc_dgs_s *d) {
  struct szc_dgsw_s *dd = (struct szc_dgsw_s *)d;
  size_t start = dd->bitlen >> 3;
  size_t end = ((dd->bitlen + count) >> 3) + ((dd->bitlen + count) % 8 == 0 ? 0 : 1);
  if (end > dd->maxlen) return 1;

  dd->val = szc_realloc(dd->val, end);
  _szcpy(cdef_SZ_b2, dd->val + start, target, count, dd->bitlen % 8);
  dd->bitlen += count;
  return 0;
}

static inline int szcyy_o_w(size_t count, uint8_t *target, struct szc_dgs_s *d) {
  struct szc_dgsw_s *dd = (struct szc_dgsw_s *)d;
  dd->bitlen += dd->bitlen % 8 == 0 ? 0 : (8 - (dd->bitlen % 8));
  size_t start = dd->bitlen >> 3;
  size_t end = start + count;
  if (end > dd->maxlen) return 1;

  dd->val = szc_realloc(dd->val, end);
  _szcpy(cdef_SZ_o, dd->val + start, target, count, 0);
  dd->bitlen += count << 3;
  return 0;
}

static inline int szcyy_o2_w(size_t count, uint8_t *target, struct szc_dgs_s *d) {
  struct szc_dgsw_s *dd = (struct szc_dgsw_s *)d;
  dd->bitlen += dd->bitlen % 8 == 0 ? 0 : (8 - (dd->bitlen % 8));
  size_t start = dd->bitlen >> 3;
  size_t end = start + count;
  if (end > dd->maxlen) return 1;

  dd->val = szc_realloc(dd->val, end);
  _szcpy(cdef_SZ_o2, dd->val + start, target, count, 0);
  dd->bitlen += count << 3;
  return 0;
}

int szcy_w(uint8_t typ, size_t count, szcv_t target, struct szc_dgs_s *d) {
  if (count == 0) return 0;
  if (count > sizeof(szcv_t)) return 1;
  switch (typ) {
    case cdef_SZ_o:
      return szcyy_o_w(count, (uint8_t *)&target, d);
    case cdef_SZ_o2:
      return szcyy_o2_w(count, (uint8_t *)&target, d);
    case cdef_SZ_b:
      return szcyy_b_w(count, (uint8_t *)&target, d);
    case cdef_SZ_b2:
      return szcyy_b2_w(count, (uint8_t *)&target, d);
    default:
      return 1;
  }
}

int szcyy_w(uint8_t typ, size_t count, uint8_t *target, struct szc_dgs_s *d) {
  if (count == 0) return 0;
  int i;
  switch (typ) {
    case cdef_SZ_o:
      return szcyy_o_w(count, target, d);
    case cdef_SZ_o2:
      return szcyy_o2_w(count, target, d);
    case cdef_SZ_b:
      return szcyy_b_w(count, target, d);
    case cdef_SZ_b2:
      return szcyy_b2_w(count, target, d);
    default:
      return 1;
  }
  return 0;
}

int szcmlc_w(void **target, size_t sz) { return 0; }

int szcrealc_w(void **target, size_t sz) { return 0; }

void *szcmemset_w(uint8_t *s, int c, size_t sz) { return s; }

void szcfree_w(void *target) { return; }

void szcfree2_w(void **target_p) { return; }

int szcyf_w(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d) {
  struct szc_dgsw_s *dd = (struct szc_dgsw_s *)d;
  return f(dd->dga1, target_ex, d);
}

int szcys_val_w(struct szc_dgs_s *target, struct szc_dgs_s *d) {
  struct szc_dgsw_s *dd_t = (struct szc_dgsw_s *)target;
  return szcyy_w(cdef_SZ_b, dd_t->bitlen, dd_t->val, d);
}
