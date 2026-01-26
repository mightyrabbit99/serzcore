#ifndef HELLOENDEC_H
#define HELLOENDEC_H

#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

#include "hello_stru.h"

ssize_t hello_deserialize(struct hello_s *p, uint8_t *data, size_t datasz);
ssize_t hello_serialize(struct hello_s *p, uint8_t *buf, size_t bufsz);

#endif  // HELLOENDEC_H
