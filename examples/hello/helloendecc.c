SZFDECL_STATIC(struct, inner_world_s, p) {
  szcyy_ex(szc_dtyp_o, sizeof(p->c), &p->c, "c", szc_extyp_int);
  szclvp_ex(szc_dtyp_o, p->data_len, 100, p->data, "data", szc_extyp_data);
  return 0;
}

SZFDECL_STATIC(struct, hello_s, p) {
  szcyy_ex(szc_dtyp_o, sizeof(p->a), &p->a, "a", szc_extyp_int);
  szcfn_ex(inner_world_s, &p->world1, "world1");
  return 0;
}
