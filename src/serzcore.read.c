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
  void *f_ctx;
  hashset_t hs;
};

szcmode_t szc_get_mode_r(void) {
  return szcmode_read;
}

szcmode2_t szc_get_mode2_r(struct szc_dgs_s *d) {
  return szcmode2_static;
}

struct szc_dgs_s *szc_init_r(void) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)szc_malloc(sizeof(struct szc_dgsr_s));
  if (dd == NULL) return NULL;
  *dd = (struct szc_dgsr_s){0};
  dd->dga1 = &szca_r;
  dd->hs = hashset_create();
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

const uint8_t *szc_retrieve_val_r(struct szc_dgs_s *d, size_t *len) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  *len = szc_get_len_r(d);
  return dd->val;
}

void szc_set_val_r(struct szc_dgs_s *d, size_t len, uint8_t *val) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  dd->maxlen = len;
  dd->val = val;
}

void szc_destruct_r(struct szc_dgs_s *d) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  hashset_destroy(dd->hs);
  szc_free(d);
}

void szc_set_ctx_r_ex(struct szc_dgs_s *d, void *ctx1, ...) {
  return;
}

int szc_get_fieldlen_r_ex(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, size_t maxlen, struct szc_dgs_s *d, const char *name, szc_extyp_t extyp, ...) {
  return 0;
}

int szcy_r(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d) {
  if (count == 0) return 0;
  if (typ >= _szc_dtyp_max) return 1;
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  if (szc_typ_is_octal(typ)) dd->bitlen += dd->bitlen % 8 == 0 ? 0 : (8 - (dd->bitlen % 8));
  size_t start = dd->bitlen >> 3;
  size_t end;
  if (szc_typ_is_octal(typ))
    end = start + count;
  else
    end = ((dd->bitlen + count) >> 3) + ((dd->bitlen + count) % 8 == 0 ? 0 : 1);
  if (end > dd->maxlen) return 1;

  if (_szcmp(typ, target, dd->val + start, count, szc_typ_is_octal(typ) ? 0 : dd->bitlen % 8)) {
    return 1;
  }
  dd->bitlen += szc_count_bit(typ, count);
  return 0;
}

int szcyy_r(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d) {
  if (count == 0) return 0;
  if (typ >= _szc_dtyp_max) return 1;
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  if (szc_typ_is_octal(typ)) dd->bitlen += dd->bitlen % 8 == 0 ? 0 : (8 - (dd->bitlen % 8));
  size_t start = dd->bitlen >> 3;
  size_t end;
  if (szc_typ_is_octal(typ))
    end = start + count;
  else
    end = ((dd->bitlen + count) >> 3) + ((dd->bitlen + count) % 8 == 0 ? 0 : 1);
  if (end > dd->maxlen) return 1;
  _szcpy_r(typ, target, dd->val + start, count, szc_typ_is_octal(typ) ? 0 : dd->bitlen % 8);
  dd->bitlen += szc_count_bit(typ, count);
  return 0;
}

int szcy_r_ex(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, const char *name, szc_extyp_t extyp, ...) {
  return szcy_r(typ, count, target, d);
}

int szcyy_r_ex(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, const char *name, szc_extyp_t extyp, ...) {
  return szcyy_r(typ, count, target, d);
}

int szcmlc_r(void **target, size_t sz, struct szc_dgs_s *d) {
  if (sz == 0) {
    *target = NULL;
    return 0;
  }
  *target = (void *)szc_malloc(sz);
  if (*target == NULL) return 1;
  memset(*target, 0, sz);
  return 0;
}

int szcrealc_r(void **target, size_t sz, struct szc_dgs_s *d) {
  if (sz == 0) {
    *target = NULL;
    return 0;
  }
  *target = (void *)szc_realloc(*target, sz);
  if (*target == NULL) return 1;
  return 0;
}

void *szcmemset_r(uint8_t *s, int c, size_t sz, struct szc_dgs_s *d) {
  return memset(s, c, sz);
}

void szcfree_r(void *target, struct szc_dgs_s *d) {
  if (target) szc_free(target);
}

void szcfree2_r(void **target_p, struct szc_dgs_s *d) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  szc_free(*target_p);
  if (hashset_is_member(dd->hs, *target_p)) {
    hashset_remove(dd->hs, *target_p);
    szc_free(target_p);
  } else {
    *target_p = NULL;
  }
}

void **szcwrapp_r(void **target_p, struct szc_dgs_s *d) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  if (hashset_is_member(dd->hs, *target_p)) return NULL;
  void **ptp = (void **)szc_malloc(sizeof(void *));
  *ptp = *target_p;
  hashset_add(dd->hs, *ptp);
  return ptp;
}

void szc_ptop_r(void **target_p, struct szc_dgs_s *d) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  if (hashset_is_member(dd->hs, *target_p)) {
    hashset_remove(dd->hs, *target_p);
    szc_free(target_p);
  }
}

int szcyf_r(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d, void *ctx) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  dd->f = f;
  dd->target_ex = target_ex;
  dd->f_ctx = ctx;
  return 0;
}

int szcys_val_r(struct szc_dgs_s *target, struct szc_dgs_s *d) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  struct szc_dgsr_s *dd_t = (struct szc_dgsr_s *)target;
  int ans = 0;
  if (dd_t->maxlen > 0) {
    if (dd_t->maxlen > dd->maxlen - (dd->bitlen >> 3)) return 1;
    szc_set_val_r(target, dd_t->maxlen, &dd->val[dd->bitlen >> 3]);
    ans = dd_t->f(dd_t->dga1, dd_t->target_ex, d, dd_t->f_ctx);
    if (ans) return ans;
    dd->bitlen += dd_t->bitlen;
  } else {
    ans = dd_t->f(dd_t->dga1, dd_t->target_ex, d, dd_t->f_ctx);
    if (ans) return ans;
  }
  return 0;
}

int szcys_val_r_ex(struct szc_dgs_s *target, struct szc_dgs_s *d, const char *name, int arr_i) {
  return szcys_val_r(target, d);
}

int szcyff_r(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d, void *ctx) {
  struct szc_dgsr_s *dd = (struct szc_dgsr_s *)d;
  return f(dd->dga1, target_ex, d, ctx);
}

int szcyff_r_ex(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d, void *ctx, const char *name, int arr_i) {
  return szcyff_r(f, target_ex, d, ctx);
}
