#ifndef HELLO_STRU_H
#define HELLO_STRU_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define printstru1(f, arg, prefix, format, ...) do {\
  f(arg, prefix);\
  f(arg, format, ## __VA_ARGS__);\
} while (0)
#define printstru2(f, arg, data, datasz) do {\
  for (size_t i__ = 0; i__ < (datasz); i__++)\
    f(arg, " %.2x", (data)[i__]);\
  f(arg, "\n");\
} while (0)

struct inner_world_s {
  int c;
  char *data;
  size_t data_len;
};

static inline void destroy_inner_world_s(struct inner_world_s *world) {
  if (world == NULL) return;
  free(world->data);
}

static inline void delete_inner_world_s(struct inner_world_s *world) {
  if (world == NULL) return;
  destroy_inner_world_s(world);
  free(world);
}

static inline void printstru_inner_world_s(struct inner_world_s *s, void (*f)(void *, const char *format, ...), void *arg, const char *prefix) {
  printstru1(f, arg, prefix, "c: %d\n", s->c);
  printstru1(f, arg, prefix, "data_len: %ld\n", s->data_len);
  printstru1(f, arg, prefix, "data: ");
  printstru2(f, arg, s->data, s->data_len);
}

static inline int filldummy_inner_world_s(struct inner_world_s *world) {
  if (world == NULL) return -1;
  world->c = 101;
  world->data = malloc(12);
  memset(world->data, 0, 12);
  strcpy(world->data, "test");
  world->data_len = strlen(world->data);
  return 0;
}

struct hello_s {
  int a;
  struct inner_world_s world1;
};

static inline void destroy_hello_s(struct hello_s *hello) {
  if (hello == NULL) return;
  destroy_inner_world_s(&hello->world1);
}

static inline void delete_hello_s(struct hello_s *hello) {
  if (hello == NULL) return;
  destroy_hello_s(hello);
  free(hello);
}

static inline struct hello_s *new_hello_s(int a) {
  struct hello_s *ans = (struct hello_s *)malloc(sizeof(struct hello_s));
  if (ans == NULL) return NULL;
  ans->a = a;
  return ans;
}

static inline void printstru_hello_s(struct hello_s *s, void (*f)(void *, const char *format, ...), void *arg, const char *prefix) {
  char prefix2[strlen(prefix) + 30];
  printstru1(f, arg, prefix, "a: %d\n", s->a);
  snprintf(prefix2, sizeof(prefix2), "%s%s", prefix, "a.world1.");
  printstru_inner_world_s(&s->world1, f, arg, prefix2);
}

static inline int filldummy_hello_s(struct hello_s *hello) {
  if (hello == NULL) return -1;
  hello->a = 42;
  filldummy_inner_world_s(&hello->world1);
  return 0;
}

static inline struct hello_s *createdummy_hello_s(void) {
  struct hello_s *ans = new_hello_s(0);
  if (ans == NULL) return NULL;
  if (filldummy_hello_s(ans)) {
    delete_hello_s(ans);
    return NULL;
  }
  return ans;
}

#endif // HELLO_STRU_H
