#include "protocol.h"
#include <stddef.h>
#include <stdio.h>

#define CLI_VERSION "v0.0.0 (init)"
#define CLI_PROTOCOL_VERSION "not developed"
#define PR_VERSIONS_TRACKED 3

// v_a[0] = cli
// v_a[1] = protocol
// v_a[2] = fw
static char version_array[PR_VERSIONS_TRACKED][PR_VERSION_STRING_BUFFER];

pr_ret_t pr_status_sys(pr_status_sys_t *status) {
  (void)status;
  return PR_FAIL;
}
pr_ret_t pr_status_per(const char *peripheral, pr_status_per_t *status);

// TODO: implement firmware version request
static int get_firmware_version(char *firmware_version, size_t buf) {
  (void)firmware_version;
  (void)buf;
  return 1;
}

// TODO: implement protocol version request + comparison
static int get_protocol_version(char *protocol_version, size_t buf) {
  (void)protocol_version;
  (void)buf;

  return 1;
}

pr_ret_t pr_version(uint8_t *fw, uint8_t *protocol, char *fw_str,
                    char *protocol_str) {
  // Request the firmware version from the MCU
  char firmware_version[PR_VERSION_STRING_BUFFER];
  *fw = get_firmware_version(firmware_version, PR_VERSION_STRING_BUFFER);

  // Compare the assumed protocol version of the CLI vs MCU's
  char protocol_version[PR_VERSION_STRING_BUFFER];
  int ret = get_protocol_version(protocol_version, PR_VERSION_STRING_BUFFER);
  switch (ret) {
  case PR_VERSION_MATCH:
    *protocol = 0;
    break;
  case PR_VERSION_FAIL:
    *protocol = 1;
    break;
  case PR_VERSION_CLASH:
    *protocol = 2;
    break;
  }

  return PR_OK;
}

void pr_watch(void) {
  while (1) {
    printf("yo\n");
  }
}
