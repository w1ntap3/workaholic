#include "protocol.h"
#include <stdint.h>

#define PR_MAGIC_BYTES 3

struct pr_frame {
  uint8_t magic[PR_MAGIC_BYTES];
  uint8_t cmd;
  uint16_t len;
  uint16_t crc;
};
