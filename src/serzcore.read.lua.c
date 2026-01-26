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

struct szc_dgsr_lua_s {
  struct szc_dga_s *dga1;
  unsigned long long int bitlen;
  size_t maxlen;
  uint8_t *val;
  lua_State *L;
};

szcmode_t szc_get_mode_r_lua(void) { return szcmode_read; }

struct szc_dgs_s *szc_init_r_lua(void) {
  struct szc_dgsr_lua_s *dd = (struct szc_dgsr_lua_s *)szc_malloc(sizeof(struct szc_dgsr_lua_s));
  if (dd == NULL) return NULL;
  *dd = (struct szc_dgsr_lua_s){0};
  dd->dga1 = &szca_r;
  return (struct szc_dgs_s *)dd;
}

void szc_fprint_r_lua(FILE *stream, struct szc_dgs_s *d) {
  struct szc_dgsr_lua_s *dd = (struct szc_dgsr_lua_s *)d;
  int i;
  for (i = 0; i < dd->bitlen >> 3; i++) {
    fprintf(stream, "%.2x", dd->val[i]);
  }
}

size_t szc_get_len_r_lua(struct szc_dgs_s *d) {
  struct szc_dgsr_lua_s *dd = (struct szc_dgsr_lua_s *)d;
  return (size_t)dd->bitlen >> 3;
}

void szc_set_maxlen_r_lua(struct szc_dgs_s *d, size_t maxlen) {
  struct szc_dgsr_lua_s *dd = (struct szc_dgsr_lua_s *)d;
  dd->maxlen = maxlen;
}

size_t szc_get_maxlen_r_lua(struct szc_dgs_s *d) {
  struct szc_dgsr_lua_s *dd = (struct szc_dgsr_lua_s *)d;
  return dd->maxlen;
}

size_t szc_get_val_r_lua(struct szc_dgs_s *d, size_t len, uint8_t *val_ptr) {
  struct szc_dgsr_lua_s *dd = (struct szc_dgsr_lua_s *)d;
  size_t ll = MIN(szc_get_len_r_lua(d), len);
  memcpy(val_ptr, dd->val, ll);
  return ll;
}

void szc_set_val_r_lua(struct szc_dgs_s *d, size_t len, uint8_t *val) {
  struct szc_dgsr_lua_s *dd = (struct szc_dgsr_lua_s *)d;
  dd->maxlen = len;
  dd->val = val;
}

void szc_destruct_r_lua(struct szc_dgs_s *d) { szc_free(d); }

void szc_set_ctx_r_ex_lua(struct szc_dgs_s *d, void *ctx1) {
  struct szc_dgsr_lua_s *dd = (struct szc_dgsr_lua_s *)d;
  dd->L = (lua_State *)ctx1;
}

int szc_get_fieldlen_r_ex_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, szc_extyp_t extyp, const char *name) {
  return 0;
}

int szcy_r_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d) {
  if (count == 0) return 1;
  if (typ >= _cdef_SZ_max) return 1;
  struct szc_dgsr_lua_s *dd = (struct szc_dgsr_lua_s *)d;
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

int szcyy_r_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d) {
  if (count == 0) return 0;
  if (typ >= _cdef_SZ_max) return 1;
  struct szc_dgsr_lua_s *dd = (struct szc_dgsr_lua_s *)d;
  if (szc_typ_is_octal(typ)) dd->bitlen += dd->bitlen % 8 == 0 ? 0 : (8 - (dd->bitlen % 8));
  size_t start = dd->bitlen >> 3;
  size_t end;
  if (szc_typ_is_octal(typ))
    end = start + count;
  else
    end = ((dd->bitlen + count) >> 3) + ((dd->bitlen + count) % 8 == 0 ? 0 : 1);
  if (end > dd->maxlen) return 1;
  _szcpy(typ, target, dd->val + start, count, szc_typ_is_octal(typ) ? 0 : dd->bitlen % 8);
  dd->bitlen += szc_count_bit(typ, count);
  return 0;
}

int szcy_r_ex_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, szc_extyp_t extyp, const char *name) {
  if (count == 0) return 0;
  if (typ >= _cdef_SZ_max) return 1;
  struct szc_dgsr_lua_s *dd = (struct szc_dgsr_lua_s *)d;
  if (szc_typ_is_octal(typ)) dd->bitlen += dd->bitlen % 8 == 0 ? 0 : (8 - (dd->bitlen % 8));
  size_t start = dd->bitlen >> 3;
  size_t end;
  if (szc_typ_is_octal(typ))
    end = start + count;
  else
    end = ((dd->bitlen + count) >> 3) + ((dd->bitlen + count) % 8 == 0 ? 0 : 1);
  if (end > dd->maxlen) return 1;
  
  uint8_t target2[szc_count_oct(typ, count)];
  _szcpy(typ, target2, dd->val + start, count, szc_typ_is_octal(typ) ? 0 : dd->bitlen % 8);
  int res = _szclua_r(dd->L, extyp, name, target2, sizeof(target2));
  if (res) return res;
  dd->bitlen += szc_count_bit(typ, count);
  return 0;
}

int szcyy_r_ex_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, szc_extyp_t extyp, const char *name) {
  return szcy_r_ex_lua(typ, count, target, d, extyp, name);
}

int szcmlc_r_lua(void **target, size_t sz) {
  if (sz == 0) {
    *target = NULL;
    return 0;
  }
  *target = (void *)szc_malloc(sz);
  if (*target == NULL) return 1;
  memset(*target, 0, sz);
  return 0;
}

int szcrealc_r_lua(void **target, size_t sz) {
  if (sz == 0) {
    *target = NULL;
    return 0;
  }
  *target = (void *)szc_realloc(*target, sz);
  if (*target == NULL) return 1;
  return 0;
}

void *szcmemset_r_lua(uint8_t *s, int c, size_t sz) { return memset(s, c, sz); }

void szcfree_r_lua(void *target) {
  if (target) szc_free(target);
}

void szcfree2_r_lua(void **target_p) {
  if (*target_p) szc_free(*target_p);
  *target_p = NULL;
}

void szc_ptop_r_ex_lua(void **target_p) {
  szcfree2_r_lua(target_p);
}

int szcyf_r_lua(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d) {
  return 0;
}

int szcys_val_r_lua(struct szc_dgs_s *target, struct szc_dgs_s *d) {
  return 0;
}

int szcyff_r_lua(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d) {
  return 0;
}

int szcyff_r_ex_lua(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d, const char *name) {
  struct szc_dgsr_lua_s *dd = (struct szc_dgsr_lua_s *)d;
  lua_pushstring(dd->L, name);
  lua_newtable(dd->L);
  if (f(dd->dga1, target_ex, d)) {
    lua_pop(dd->L, 2);
    return 1;
  }
  lua_settable(dd->L, -3);
  return 0;
}
