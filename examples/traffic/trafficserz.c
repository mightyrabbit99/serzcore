#include "trafficserz.h"

#include "serzcore_utils.h"

SZFDECL_STATIC(struct, null_s, p, dst) {
  szcy(cdef_SZ_o, 3, 0, dst);
  return 0;
szcfail:
  return 1;
}

SZFDECL_STATIC(struct, car_s, p, dst) {
  szcyy(cdef_SZ_o, sizeof(p->brand), p->brand, dst);
  szcyy(cdef_SZ_o, sizeof(p->plate), p->plate, dst);
  szcyy(cdef_SZ_o, sizeof(p->passengers), &p->passengers, dst);
  szcmlcyy(cdef_SZ_o, sizeof(*p->secret), p->secret, dst);
  return 0;
szcfail:
  return 1;
}

SZFDECL_STATIC(struct, ship_s, p, dst) {
  szcyy(cdef_SZ_o, sizeof(p->name), p->name, dst);
  szclvstr(cdef_SZ_o, 100, p->manufacturer, dst);
  szcyy(cdef_SZ_o, sizeof(p->tonnage), &p->tonnage, dst);
  szclvp(cdef_SZ_o, p->data_len, p->data, dst);
  return 0;
szcfail:
  return 1;
}

static szc_ff_t get_vehicle_dgf(vetyp_t typ) {
  switch (typ) {
    case vetyp_null:
      return SZFNAME(null_s);
    case vetyp_car:
      return SZFNAME(car_s);
    case vetyp_ship:
      return SZFNAME(ship_s);
    default:
      return NULL;
  }
}

static void *get_vehicle_dgtg(union vehicle_u *vehicle, vetyp_t typ) {
  switch (typ) {
    case vetyp_null:
      return (void *)&vehicle->null;
    case vetyp_car:
      return (void *)&vehicle->car;
    case vetyp_ship:
      return (void *)&vehicle->ship;
    default:
      return NULL;
  }
}

SZFDECL_STATIC(struct, traffic_s, p, dst) {
  int i;
  for (i = 0;;) {
    szcrealc((void **)&p->vehicle_arr, sizeof(struct vehicle_s) * (i + 1));
    szcmemset((uint8_t *)&p->vehicle_arr[i], 0, sizeof(struct vehicle_s));
    szcyy(cdef_SZ_o, sizeof(vetyp_t), &p->vehicle_arr[i].typ, dst);
    szc_ff_t ff = get_vehicle_dgf(p->vehicle_arr[i].typ);
    if (ff == NULL) szcthrowerr();
    void *tgt = get_vehicle_dgtg(&p->vehicle_arr[i].v, p->vehicle_arr[i].typ);
    if (tgt == NULL) szcthrowerr();
    szclvrcrse(cdef_SZ_o, uint8_t, ff, tgt, dst);
    if (p->vehicle_arr[i++].typ == vetyp_null) break;
  }
  if (szc_get_mode() == szcmode_read) p->num_of_vehicles = i;
  if (i == 0) szcdelete(p->vehicle_arr);

  return 0;
szcfail:
  szcdelete(p->vehicle_arr);
  return 1;
}

ssize_t traffic_deserialize(struct traffic_s *p, uint8_t *data, size_t datasz) {
  size_t ll;
  SZFREAD(traffic_s, ll, p, data, datasz);
  return ll;
szcfail:
  return -1;
}

ssize_t traffic_serialize(struct traffic_s *p, uint8_t *buf, size_t bufsz) {
  size_t ll;
  SZFWRITE(traffic_s, ll, p, buf, bufsz);
  return ll;
szcfail:
  return -1;
}
