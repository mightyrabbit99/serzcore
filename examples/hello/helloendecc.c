SZFDECL_STATIC(struct, inner_world_s, p) {
  szcyy_ex(cdef_SZ_o, sizeof(p->c), &p->c, szc_extyp_int, "c");
  szclvp_ex(cdef_SZ_o, p->data_len, 100, p->data, szc_extyp_data, "data");
  return 0;
}

SZFDECL_STATIC(struct, hello_s, p) {
  szcyy_ex(cdef_SZ_o, sizeof(p->a), &p->a, szc_extyp_int, "a");
  szcf_ex(inner_world_s, &p->world1, "world1");
  return 0;
}
