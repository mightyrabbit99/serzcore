#include "helloendec.h"

#include "serzcore_utils.h"

#include "helloendecc.c"

ssize_t hello_deserialize(struct hello_s *p, uint8_t *data, size_t datasz) {
  size_t ll;
  SZFREAD(hello_s, ll, p, data, datasz);
  return ll;
}

ssize_t hello_serialize(struct hello_s *p, uint8_t *buf, size_t bufsz) {
  size_t ll;
  SZFWRITE(hello_s, ll, p, buf, bufsz);
  return ll;
}
