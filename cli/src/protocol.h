#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define PR_PER_NAME_LIMIT 32
#define PR_UNIT_NAME_LIMIT 32

#define PR_MAX_NUM_BYTES 8

#define CLI_VERSION "v0.0.0 (init)"
#define CLI_PROTOCOL_VERSION "not developed"
#define PR_VERSIONS_TRACKED 3 // cli, protocol, fw
#define PR_VERSION_STRING_BUFFER 64

typedef enum { PR_OK = 0, PR_FAIL } pr_ret_t;

typedef enum { PRVAL_UINT = 0, PRVAL_INT, PRVAL_DOUBLE } pr_num_t;

typedef enum {
  PR_STATE_HEALTHY = 0,
  PR_STATE_DISCONNECTED, // a connection was terminated
  PR_STATE_WEAK, // heartbeat is late (just a suspicion, not fully writing off)
  PR_STATE_DEAD, // heartbeat has either never existed (despite the peripheral
                 // being configured) or its late beyond the threshold
  PR_STATE_BELOW_RANGE,
  PR_STATE_ABOVE_RANGE
} pr_health_state;

typedef struct {
  char name[PR_PER_NAME_LIMIT]; // name[0] == (NULL) is the status for the
                                // whole system
  pr_health_state health;
  pr_num_t val_type;
  uint8_t value[PR_MAX_NUM_BYTES];
  uint8_t min[PR_MAX_NUM_BYTES];
  uint8_t max[PR_MAX_NUM_BYTES];
  uint64_t last_sample; // delta in ticks
  char unit[PR_UNIT_NAME_LIMIT];
  pr_num_t read_type;
  uint8_t reading[PR_MAX_NUM_BYTES];
} pr_status_per_t;

typedef struct {
  uint16_t id;
  char name[PR_PER_NAME_LIMIT];
  pr_health_state health;
  pr_num_t val_type;
  uint8_t value[PR_MAX_NUM_BYTES];
  char unit[PR_UNIT_NAME_LIMIT];
} pr_row_per;

typedef struct {
  pr_health_state health;
  uint64_t uptime;
  pr_row_per peripherals[];
} pr_status_sys_t;

// v_a[0] = cli
// v_a[1] = protocol
// v_a[2] = fw
extern char version_array[PR_VERSIONS_TRACKED][PR_VERSION_STRING_BUFFER];

// writes (and blocks until) received status into variable
pr_ret_t pr_status_sys(pr_status_sys_t *status);
pr_ret_t pr_status_per(const char *peripheral, pr_status_per_t *status);
pr_ret_t pr_version();

#endif
