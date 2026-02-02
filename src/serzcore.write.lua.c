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

struct szc_dgsw_lua_s {
  struct szc_dga_s *dga1;
  unsigned long long int bitlen;
  size_t maxlen;
  uint8_t *val;
  lua_State *L;
};

szcmode_t szc_get_mode_w_lua(void) {
  return szcmode_write;
}

szcmode2_t szc_get_mode2_w_lua(struct szc_dgs_s *d) {
  return szcmode2_dynamic;
}

struct szc_dgs_s *szc_init_w_lua(void) {
  struct szc_dgsw_lua_s *dd = (struct szc_dgsw_lua_s *)szc_malloc(sizeof(struct szc_dgsw_lua_s));
  if (dd == NULL) return NULL;
  *dd = (struct szc_dgsw_lua_s){0};
  dd->maxlen = MAZ_SZ;
  dd->dga1 = &szca_w;
  return (struct szc_dgs_s *)dd;
}

void szc_fprint_w_lua(FILE *stream, struct szc_dgs_s *d) {
  struct szc_dgsw_lua_s *dd = (struct szc_dgsw_lua_s *)d;
  int i;
  for (i = 0; i < dd->bitlen >> 3; i++) {
    fprintf(stream, "%.2x", dd->val[i]);
  }
}

size_t szc_get_len_w_lua(struct szc_dgs_s *d) {
  struct szc_dgsw_lua_s *dd = (struct szc_dgsw_lua_s *)d;
  return (size_t)dd->bitlen >> 3;
}

void szc_set_maxlen_w_lua(struct szc_dgs_s *d, size_t maxlen) {
  struct szc_dgsw_lua_s *dd = (struct szc_dgsw_lua_s *)d;
  dd->maxlen = maxlen;
}

size_t szc_get_maxlen_w_lua(struct szc_dgs_s *d) {
  struct szc_dgsw_lua_s *dd = (struct szc_dgsw_lua_s *)d;
  return dd->maxlen;
}

size_t szc_get_val_w_lua(struct szc_dgs_s *d, size_t len, uint8_t *val_ptr) {
  struct szc_dgsw_lua_s *dd = (struct szc_dgsw_lua_s *)d;
  size_t ll = MIN(szc_get_len_w_lua(d), len);
  memcpy(val_ptr, dd->val, ll);
  return ll;
}

const uint8_t *szc_retrieve_val_w_lua(struct szc_dgs_s *d, size_t *len) {
  struct szc_dgsw_lua_s *dd = (struct szc_dgsw_lua_s *)d;
  *len = szc_get_len_w_lua(d);
  return dd->val;
}

void szc_set_val_w_lua(struct szc_dgs_s *d, size_t len, uint8_t *val) {
  struct szc_dgsw_lua_s *dd = (struct szc_dgsw_lua_s *)d;
  dd->bitlen = len << 3;
  uint8_t *val2 = (uint8_t *)szc_malloc(sizeof(uint8_t) * len);
  memcpy(val2, val, len);
  dd->val = val2;
}

void szc_destruct_w_lua(struct szc_dgs_s *d) {
  struct szc_dgsw_lua_s *dd = (struct szc_dgsw_lua_s *)d;
  if (dd->val) szc_free(dd->val);
  dd->val = NULL;
  szc_free(dd);
}

void szc_set_ctx_w_ex_lua(struct szc_dgs_s *d, void *ctx1, ...) {
  struct szc_dgsw_lua_s *dd = (struct szc_dgsw_lua_s *)d;
  dd->L = (lua_State *)ctx1;
}

int szc_get_fieldlen_w_ex_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, size_t maxlen, struct szc_dgs_s *d, const char *name, szc_extyp_t extyp, ...) {
  struct szc_dgsw_lua_s *dd = (struct szc_dgsw_lua_s *)d;
  size_t sz = 0;
  szc_extyp_t extyp2;
  if (extyp == szc_extyp_arr) {
    va_list argp;
    va_start(argp, extyp);
    extyp2 = va_arg(argp, int);
    va_end(argp);
  } else {
    extyp2 = extyp;
  }
  ssize_t res = _szclua_w_get_fieldlen(dd->L, extyp2, name);
  if (res < 0) return 1;
  sz = MIN(maxlen, (size_t)res);
  _szcpy_r(typ, target, (uint8_t *)&sz, count, szc_typ_is_octal(typ) ? 0 : dd->bitlen % 8);
  return 0;
}

int szcy_w_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d) {
  if (count == 0) return 0;
  if (typ >= _szc_dtyp_max) return 1;
  struct szc_dgsw_lua_s *dd = (struct szc_dgsw_lua_s *)d;
  if (szc_typ_is_octal(typ)) dd->bitlen += dd->bitlen % 8 == 0 ? 0 : (8 - (dd->bitlen % 8));
  size_t start = dd->bitlen >> 3;
  size_t end;
  if (szc_typ_is_octal(typ))
    end = start + count;
  else
    end = ((dd->bitlen + count) >> 3) + ((dd->bitlen + count) % 8 == 0 ? 0 : 1);
  uint8_t *val_p;
  if (end > dd->maxlen) return 1;
  val_p = szc_realloc(dd->val, end);
  if (val_p == NULL) return 1;
  dd->val = val_p;
  _szcpy_w(typ, dd->val + start, target, count, szc_typ_is_octal(typ) ? 0 : dd->bitlen % 8);
  dd->bitlen += szc_count_bit(typ, count);

  return 0;
}

int szcyy_w_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d) {
  return szcy_w_lua(typ, count, target, d);
}

static inline int _szcyv_w_ex_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, const char *name, szc_extyp_t extyp, va_list extyp_va) {
  if (count == 0) return 0;
  if (typ >= _szc_dtyp_max) return 1;
  struct szc_dgsw_lua_s *dd = (struct szc_dgsw_lua_s *)d;
  if (szc_typ_is_octal(typ)) dd->bitlen += dd->bitlen % 8 == 0 ? 0 : (8 - (dd->bitlen % 8));
  return _szclua_w_append(dd->L, extyp, extyp_va, name, typ, &dd->val, &dd->bitlen, dd->maxlen, count);
}

int szcy_w_ex_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, const char *name, szc_extyp_t extyp, ...) {
  va_list argp;
  va_start(argp, extyp);
  int ans = _szcyv_w_ex_lua(typ, count, target, d, name, extyp, argp);
  va_end(argp);
  return ans;
}

int szcyy_w_ex_lua(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, const char *name, szc_extyp_t extyp, ...) {
  va_list argp;
  va_start(argp, extyp);
  int ans = _szcyv_w_ex_lua(typ, count, target, d, name, extyp, argp);
  va_end(argp);
  return ans;
}

int szcmlc_w_lua(void **target, size_t sz, struct szc_dgs_s *d) {
  return 0;
}

int szcrealc_w_lua(void **target, size_t sz, struct szc_dgs_s *d) {
  return 0;
}

void *szcmemset_w_lua(uint8_t *s, int c, size_t sz, struct szc_dgs_s *d) {
  return NULL;
}

void szcfree_w_lua(void *target, struct szc_dgs_s *d) {
  return;
}

void szcfree2_w_lua(void **target_p, struct szc_dgs_s *d) {
  return;
}

void **szcwrapp_w_lua(void **target_p, struct szc_dgs_s *d) {
  return target_p;
}

void szc_ptop_w_lua(void **target_p, struct szc_dgs_s *d) {
  szcfree2_w_lua(target_p, d);
}

int szcyf_w_lua(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d, void* ctx) {
  struct szc_dgsw_lua_s *dd = (struct szc_dgsw_lua_s *)d;
  return f(dd->dga1, target_ex, d, ctx);
}

int szcys_val_w_lua(struct szc_dgs_s *target, struct szc_dgs_s *d) {
  struct szc_dgsw_lua_s *dd_t = (struct szc_dgsw_lua_s *)target;
  return szcyy_w_lua(szc_dtyp_b, dd_t->bitlen, dd_t->val, d);
}

int szcys_val_w_ex_lua(struct szc_dgs_s *target, struct szc_dgs_s *d, const char *name, int arr_i) {
  return szcys_val_w_lua(target, d);
}

int szcyff_w_lua(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d, void* ctx) {
  return 0;
}

int szcyff_w_ex_lua(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d, void* ctx, const char *name, int arr_i) {
  struct szc_dgsw_lua_s *dd = (struct szc_dgsw_lua_s *)d;
  int ans;
  if (!lua_istable(dd->L, -1)) return 1;
  lua_getfield(dd->L, -1, name);
  if (!lua_istable(dd->L, -1)) return 1;
  ans = f(dd->dga1, target_ex, d, ctx);
  lua_pop(dd->L, 1);
  return ans;
}
