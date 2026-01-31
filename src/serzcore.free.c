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

struct szc_dgsf_s {
  szc_ff_t f;
  struct szc_dga_s *dga1;
  _target_ex target_ex;
  unsigned long long int bitlen;
  size_t maxlen;
  hashset_t hs;
};

szcmode_t szc_get_mode_f(void) { return szcmode_free; }

szcmode2_t szc_get_mode2_f(void) { return szcmode2_static; }

struct szc_dgs_s *szc_init_f(void) {
  struct szc_dgsf_s *dd = (struct szc_dgsf_s *)szc_malloc(sizeof(struct szc_dgsf_s));
  if (dd == NULL) return NULL;
  *dd = (struct szc_dgsf_s){0};
  dd->dga1 = &szca_f;
  dd->hs = hashset_create();
  if (dd->hs == NULL) goto fail;
  return (struct szc_dgs_s *)dd;
fail:
  free(dd);
  return NULL;
}

void szc_fprint_f(FILE *stream, struct szc_dgs_s *d) {}

size_t szc_get_len_f(struct szc_dgs_s *d) { return 0; }

void szc_set_maxlen_f(struct szc_dgs_s *d, size_t maxlen) {
  struct szc_dgsf_s *dd = (struct szc_dgsf_s *)d;
  dd->maxlen = maxlen;
}

size_t szc_get_maxlen_f(struct szc_dgs_s *d) {
  struct szc_dgsf_s *dd = (struct szc_dgsf_s *)d;
  return dd->maxlen;
}

size_t szc_get_val_f(struct szc_dgs_s *d, size_t len, uint8_t *val_ptr) { return 0; }

const uint8_t *szc_retrieve_val_f(struct szc_dgs_s *d, size_t *len) { return NULL; }

void szc_set_val_f(struct szc_dgs_s *d, size_t len, uint8_t *val) {}

void szc_destruct_f(struct szc_dgs_s *d) {
  struct szc_dgsf_s *dd = (struct szc_dgsf_s *)d;
  hashset_destroy(dd->hs);
  szc_free(d);
}

void szc_set_ctx_f_ex(struct szc_dgs_s *d, void *ctx1, ...) { return; }

int szc_get_fieldlen_f_ex(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, size_t maxlen, struct szc_dgs_s *d, const char *name, szc_extyp_t extyp, ...) { return 0; }

int szcy_f(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d) { return 0; }

int szcyy_f(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d) { return 0; }

int szcy_f_ex(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, const char *name, szc_extyp_t extyp, ...) { return 0; }

int szcyy_f_ex(szc_dtyp_t typ, unsigned long long int count, uint8_t *target, struct szc_dgs_s *d, const char *name, szc_extyp_t extyp, ...) { return 0; }

int szcmlc_f(void **target, size_t sz, struct szc_dgs_s *d) { return 0; }

int szcrealc_f(void **target, size_t sz, struct szc_dgs_s *d) { return 0; }

void *szcmemset_f(uint8_t *s, int c, size_t sz, struct szc_dgs_s *d) { return NULL; }

void szcfree_f(void *target, struct szc_dgs_s *d) { szc_free(target); }

void szcfree2_f(void **target_p, struct szc_dgs_s *d) { szc_free(*target_p); }

void **szcwrapp_f(void **target_p, struct szc_dgs_s *d) {
  struct szc_dgsf_s *dd = (struct szc_dgsf_s *)d;
  if (hashset_is_member(dd->hs, *target_p)) return NULL;
  hashset_add(dd->hs, *target_p);
  return target_p;
}

void szc_ptop_f(void **target_p, struct szc_dgs_s *d) {
  struct szc_dgsf_s *dd = (struct szc_dgsf_s *)d;
  hashset_remove(dd->hs, *target_p);
  szcfree2_f(target_p, d);
}

int szcyf_f(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d) {
  struct szc_dgsf_s *dd = (struct szc_dgsf_s *)d;
  dd->f = f;
  dd->target_ex = target_ex;
  return 0;
}

int szcys_val_f(struct szc_dgs_s *target, struct szc_dgs_s *d) {
  struct szc_dgsf_s *dd = (struct szc_dgsf_s *)d;
  struct szc_dgsf_s *dd_t = (struct szc_dgsf_s *)target;
  int ans = 0;
  ans = dd_t->f(dd_t->dga1, dd_t->target_ex, d);
  return ans;
}

int szcys_val_f_ex(struct szc_dgs_s *target, struct szc_dgs_s *d, const char *name, int arr_i) { return szcys_val_f(target, d); }

int szcyff_f(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d) {
  struct szc_dgsf_s *dd = (struct szc_dgsf_s *)d;
  return f(dd->dga1, target_ex, d);
}

int szcyff_f_ex(szc_ff_t f, _target_ex target_ex, struct szc_dgs_s *d, const char *name, int arr_i) { return szcyff_f(f, target_ex, d); }
