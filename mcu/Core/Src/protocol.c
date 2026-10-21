#include "protocol.h"
#include <stdint.h>

#define MAGIC_BYTES 3

struct pr_frame {
  uint8_t magic[MAGIC_BYTES];
  uint8_t cmd;
  uint16_t len;
  uint16_t crc;
};
