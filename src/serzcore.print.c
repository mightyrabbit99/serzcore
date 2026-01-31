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

struct szc_dgsp_s {
  szc_ff_t f;
  struct szc_dga_s *dga1;
  _target_ex target_ex;
  unsigned long long int bitlen;
  size_t maxlen;
  uint8_t *val;
  uint8_t ptyp;
  uint8_t indent;
};

#define _print_indent(dd) do {\
  for (uint8_t i = 0; i < dd->indent; i++) printf("  ");\
} while (0)

szcmode_t szc_get_mode_p(void) { return szcmode_print; }

szcmode2_t szc_get_mode2_p(void) { return szcmode2_static; }

struct szc_dgs_s *szc_init_p(void) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)szc_malloc(sizeof(struct szc_dgsp_s));
  if (dd == NULL) return NULL;
  *dd = (struct szc_dgsp_s){0};
  dd->dga1 = &szca_p;
  return (struct szc_dgs_s *)dd;
}

void szc_fprint_p(FILE *stream, struct szc_dgs_s *d) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  int i;
  for (i = 0; i < dd->bitlen >> 3; i++) {
    fprintf(stream, "%.2x", dd->val[i]);
  }
}

size_t szc_get_len_p(struct szc_dgs_s *d) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  return (size_t)dd->bitlen >> 3;
}

void szc_set_maxlen_p(struct szc_dgs_s *d, size_t maxlen) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  dd->maxlen = maxlen;
}

size_t szc_get_maxlen_p(struct szc_dgs_s *d) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  return dd->maxlen;
}

size_t szc_get_val_p(struct szc_dgs_s *d, size_t len, uint8_t *val_ptr) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  size_t ll = MIN(szc_get_len_p(d), len);
  memcpy(val_ptr, dd->val, ll);
  return ll;
}

const uint8_t *szc_retrieve_val_p(struct szc_dgs_s *d, size_t *len) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  *len = szc_get_len_p(d);
  return dd->val;
}

void szc_set_val_p(struct szc_dgs_s *d, size_t len, uint8_t *val) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  dd->maxlen = len;
  dd->val = val;
}

void szc_destruct_p(struct szc_dgs_s *d) { szc_free(d); }

void szc_set_ctx_p_ex(struct szc_dgs_s *d, void *ctx1, ...) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  dd->ptyp = *(uint8_t *)ctx1;
}

int szc_get_fieldlen_p_ex(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, size_t maxlen, struct szc_dgs_s *d, const char *name, szc_extyp_t extyp, ...) {
  return 0;
}

int szcy_p(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d) {
  if (count == 0) return 1;
  if (typ >= _szc_dtyp_max) return 1;
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  if (dd->ptyp == szc_ptyp_struct) return 0;
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

int szcyy_p(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d) {
  if (count == 0) return 0;
  if (typ >= _szc_dtyp_max) return 1;
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  if (dd->ptyp == szc_ptyp_struct) return 0;
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

static inline int _szcyv_p_ex(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, const char *name, szc_extyp_t extyp, va_list extyp_va) {
  if (count == 0) return 0;
  if (typ >= _szc_dtyp_max) return 1;
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  if (szc_typ_is_octal(typ)) dd->bitlen += dd->bitlen % 8 == 0 ? 0 : (8 - (dd->bitlen % 8));
  size_t start = dd->bitlen >> 3;
  size_t end;
  if (szc_typ_is_octal(typ))
    end = start + count;
  else
    end = ((dd->bitlen + count) >> 3) + ((dd->bitlen + count) % 8 == 0 ? 0 : 1);
  if (dd->ptyp == szc_ptyp_stream && end > dd->maxlen) return 1;

  uint8_t *tgt = dd->ptyp == szc_ptyp_stream ? dd->val + start : target;
  _print_indent(dd);
  _szcprint(_fprintf, stdout, typ, tgt, count, szc_typ_is_octal(typ) ? 0 : dd->bitlen % 8, name, extyp, extyp_va);
  printf("\n");
  return 0;
}

int szcy_p_ex(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, const char *name, szc_extyp_t extyp, ...) {
  va_list argp;
  va_start(argp, extyp);
  int ans = _szcyv_p_ex(typ, count, target, d, name, extyp, argp);
  va_end(argp);
  return ans;
}

int szcyy_p_ex(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, const char *name, szc_extyp_t extyp, ...) {
  va_list argp;
  va_start(argp, extyp);
  int ans = _szcyv_p_ex(typ, count, target, d, name, extyp, argp);
  va_end(argp);
  return ans;
}

int szcmlc_p(void **target, size_t sz, struct szc_dgs_s *d) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  if (dd->ptyp == szc_ptyp_struct) return 0;
  if (sz == 0) {
    *target = NULL;
    return 0;
  }
  *target = (void *)szc_malloc(sz);
  if (*target == NULL) return 1;
  memset(*target, 0, sz);
  return 0;
}

int szcrealc_p(void **target, size_t sz, struct szc_dgs_s *d) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  if (dd->ptyp == szc_ptyp_struct) return 0;
  if (sz == 0) {
    *target = NULL;
    return 0;
  }
  *target = (void *)szc_realloc(*target, sz);
  if (*target == NULL) return 1;
  return 0;
}

void *szcmemset_p(uint8_t *s, int c, size_t sz, struct szc_dgs_s *d) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  if (dd->ptyp == szc_ptyp_struct) return NULL;
  return memset(s, c, sz);
}

void szcfree_p(void *target, struct szc_dgs_s *d) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  if (dd->ptyp == szc_ptyp_struct) return;
  if (target) szc_free(target);
}

void szcfree2_p(void **target_p, struct szc_dgs_s *d) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  if (dd->ptyp == szc_ptyp_struct) return;
  if (*target_p) szc_free(*target_p);
  *target_p = NULL;
}

void **szcwrapp_p(void **target_p, struct szc_dgs_s *d) {
  return target_p;
}

void szc_ptop_p(void **target_p, struct szc_dgs_s *d) { return; }

int szcyf_p(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  dd->f = f;
  dd->target_ex = target_ex;
  return 0;
}

int szcys_val_p(struct szc_dgs_s *target, struct szc_dgs_s *d) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  struct szc_dgsp_s *dd_t = (struct szc_dgsp_s *)target;
  int ans = 0;
  if (dd_t->maxlen > 0) {
    if (dd_t->maxlen > dd->maxlen - (dd->bitlen >> 3)) return 1;
    szc_set_val_p(target, dd_t->maxlen, &dd->val[dd->bitlen >> 3]);
    ans = dd_t->f(dd_t->dga1, dd_t->target_ex, target);
    if (ans) return ans;
    dd->bitlen += dd_t->bitlen;
  } else {
    ans = dd_t->f(dd_t->dga1, dd_t->target_ex, d);
    if (ans) return ans;
  }
  return 0;
}

int szcys_val_p_ex(struct szc_dgs_s *target, struct szc_dgs_s *d, const char *name, int arr_i) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  _print_indent(dd);
  printf("::%s", name);
  if (arr_i != -1) printf("[%d]", arr_i);
  printf("::\n");
  dd->indent++;
  int ans = szcys_val_p(target, d);
  dd->indent--;
  if (ans == 0) {
    _print_indent(dd);
    printf("::%s", name);
    if (arr_i != -1) printf("[%d]", arr_i);
    printf("::\n");
  }
  return ans;
}

int szcyff_p(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  return f(dd->dga1, target_ex, d);
}

int szcyff_p_ex(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d, const char *name, int arr_i) {
  struct szc_dgsp_s *dd = (struct szc_dgsp_s *)d;
  _print_indent(dd);
  printf("::%s", name);
  if (arr_i != -1) printf("[%d]", arr_i);
  printf("::\n");
  dd->indent++;
  int ans = szcyff_p(f, target_ex, d);
  dd->indent--;
  if (ans == 0) {
    _print_indent(dd);
    printf("::%s", name);
    if (arr_i != -1) printf("[%d]", arr_i);
    printf("::\n");
  }
  return ans;
}
