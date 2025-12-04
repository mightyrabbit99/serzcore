#ifndef TRAFFICSERZ_H
#define TRAFFICSERZ_H

#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

#include "traffic_stru.h"

ssize_t traffic_deserialize(struct traffic_s *p, uint8_t *data, size_t datasz);
ssize_t traffic_serialize(struct traffic_s *p, uint8_t *buf, size_t bufsz);

#endif  // TRAFFICSERZ_H
