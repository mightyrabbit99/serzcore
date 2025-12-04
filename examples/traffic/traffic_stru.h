#ifndef TRAFFIC_STRU_H
#define TRAFFIC_STRU_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t vetyp_t;

enum vehicle_typ_e {
  vetyp_null,
  vetyp_car,
  vetyp_ship,
};

struct null_s {};

struct car_s {
  char brand[10];
  char plate[10];
  int passengers;
  int *secret;
};

inline static void car_s_print(struct car_s *car, void (*f)(void *, const char *format, ...), void *arg) {
  f(arg, "car brand: %s\n", car->brand);
  f(arg, "car plate: %s\n", car->plate);
  f(arg, "car passangers: %d\n", car->passengers);
  f(arg, "car secret: %d\n", *car->secret);
}

inline static int car_s_filldummy(struct car_s *car) {
  strncpy(car->brand, "toyota", sizeof(car->brand));
  strncpy(car->plate, "AAA1234", sizeof(car->plate));
  car->secret = (int *)malloc(sizeof(int));
  *car->secret = 42;
  car->passengers = 3;
  return 0;
}

inline static void car_s_destroy(struct car_s *car) {
  if (car->secret) free(car->secret);
  car->secret = NULL;
}

struct ship_s {
  char name[10];
  int tonnage;
};

inline static void ship_s_print(struct ship_s *ship, void (*f)(void *, const char *format, ...), void *arg) {
  f(arg, "ship name: %s\n", ship->name);
  f(arg, "ship tonnage: %d\n", ship->tonnage);
}

inline static int ship_s_filldummy(struct ship_s *ship) {
  strncpy(ship->name, "alice", sizeof(ship->name));
  ship->tonnage = 101;
  return 0;
}

inline static void ship_s_destroy(struct ship_s *ship) {}

union vehicle_u {
  struct null_s null;
  struct car_s car;
  struct ship_s ship;
};

struct vehicle_s {
  vetyp_t typ;
  union vehicle_u v;
};

inline static void vehicle_s_print(struct vehicle_s *vehicle, void (*f)(void *, const char *format, ...), void *arg) {
  f(arg, "vehicle typ: %d\n", vehicle->typ);
  switch (vehicle->typ) {
    case vetyp_null:
      return;
    case vetyp_car:
      return car_s_print(&vehicle->v.car, f, arg);
    case vetyp_ship:
      return ship_s_print(&vehicle->v.ship, f, arg);
    default:
      return;
  }
}

inline static void vehicle_s_destroy(struct vehicle_s *vehicle) {
  switch (vehicle->typ) {
    case vetyp_null:
      return;
    case vetyp_car:
      return car_s_destroy(&vehicle->v.car);
    case vetyp_ship:
      return ship_s_destroy(&vehicle->v.ship);
    default:
      return;
  }
}

inline static int vehicle_s_filldummy(struct vehicle_s *vehicle, vetyp_t typ) {
  vehicle->typ = typ;
  switch (vehicle->typ) {
    case vetyp_null:
      return 0;
    case vetyp_car:
      return car_s_filldummy(&vehicle->v.car);
    case vetyp_ship:
      return ship_s_filldummy(&vehicle->v.ship);
    default:
      return -1;
  }
}

struct traffic_s {
  int num_of_vehicles;
  struct vehicle_s *vehicle_arr;
};

inline static void traffic_s_print(struct traffic_s *traffic, void (*f)(void *, const char *format, ...), void *arg) {
  f(arg, "traffic num_of_vehicles: %d\n", traffic->num_of_vehicles);
  for (int i = 0; i < traffic->num_of_vehicles; i++) {
    vehicle_s_print(&traffic->vehicle_arr[i], f, arg);
  }
}

inline static struct traffic_s *traffic_s_new(void) {
  struct traffic_s *ans = (struct traffic_s *)malloc(sizeof(struct traffic_s));
  if (ans == NULL) return NULL;
  ans->num_of_vehicles = 0;
  ans->vehicle_arr = NULL;
  return ans;
}

inline static void traffic_s_destroy(struct traffic_s *traffic) {
  for (int i = 0; i < traffic->num_of_vehicles; i++) {
    vehicle_s_destroy(&traffic->vehicle_arr[i]);
  }
  free(traffic->vehicle_arr);
}

inline static void traffic_s_delete(struct traffic_s *traffic) {
  traffic_s_destroy(traffic);
  free(traffic);
}

inline static int traffic_s_filldummy(struct traffic_s *traffic, uint8_t tlvt_nr, vetyp_t *tlvts) {
  traffic->num_of_vehicles = tlvt_nr;
  traffic->vehicle_arr = (struct vehicle_s *)calloc(traffic->num_of_vehicles, sizeof(struct vehicle_s));
  for (int i = 0; i < traffic->num_of_vehicles; i++) {
    if (-1 == vehicle_s_filldummy(&traffic->vehicle_arr[i], tlvts[i])) return -1;
  }
  return 0;
}

inline static struct traffic_s *traffic_s_createdummy(uint8_t tlvt_nr, vetyp_t *tlvts) {
  struct traffic_s *ans = traffic_s_new();
  if (-1 == traffic_s_filldummy(ans, tlvt_nr, tlvts)) {
    traffic_s_delete(ans);
    return NULL;
  }
  return ans;
}

#endif  // TRAFFIC_STRU_H
