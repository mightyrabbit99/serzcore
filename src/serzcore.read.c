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

struct szc_dgsr_s {
  szc_ff_t f;
  struct szc_dga_s *dga1;
  _target_ex target_ex;
  unsigned long long int bitlen;
  size_t maxlen;
  uint8_t *val;
};

szcmode_t szc_get_mode_r(void) { return szcmode_read; }

struct szc_dgs_s *szc_init_r(void) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)szc_malloc(sizeof(struct szc_dgsr_s));
  if (dd == NULL) return NULL;
  *dd = (struct szc_dgsr_s){0};
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

int szcy_r(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d) {
  if (count == 0) return 1;
  if (typ >= _cdef_SZ_max) return 1;
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  if (szc_typ_is_octal(typ)) {
    if (((dd->bitlen >> 3) + (dd->bitlen % 8 == 0 ? 0 : 1) + count) > dd->maxlen) return 1;
    dd->bitlen += dd->bitlen % 8 == 0 ? 0 : (8 - (dd->bitlen % 8));
    dd->bitlen += count << 3;
  } else {
    if (((dd->bitlen + count) >> 3) > dd->maxlen) return 1;
    dd->bitlen += count;
  }
  return 0;
}

int szcyy_r(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d) {
  if (count == 0) return 0;
  if (typ >= _cdef_SZ_max) return 1;
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  if (szc_typ_is_octal(typ)) dd->bitlen += dd->bitlen % 8 == 0 ? 0 : (8 - (dd->bitlen % 8));
  size_t start = dd->bitlen >> 3;
  size_t end;
  if (szc_typ_is_octal(typ))
    end = start + count;
  else
    end = start + (count >> 3) + (count % 8 == 0 ? 0 : 1);
  if (end > dd->maxlen) return 1;
  _szcpy(typ, target, dd->val + start, count, szc_typ_is_octal(typ) ? 0 : dd->bitlen % 8);
  dd->bitlen += szc_typ_is_octal(typ) ? count << 3 : count;
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

void *szcmemset_r(uint8_t *s, int c, size_t sz) { return memset(s, c, sz); }

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
