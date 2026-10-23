#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <time.h>

#define PR_PER_NAME_LIMIT 32
#define PR_UNIT_NAME_LIMIT 32

#define PR_MAX_NUM_BYTES 8

#define PR_VERSION_STRING_BUFFER 64

typedef enum {
  PR_OK = 0,
  PR_FAIL,
  PR_VERSION_MATCH,
  PR_VERSION_FAIL,
  PR_VERSION_CLASH
} pr_ret_t;

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

typedef struct pr_row_per_t {
  uint16_t id;
  char name[PR_PER_NAME_LIMIT];
  pr_health_state health;
  pr_num_t val_type;
  uint8_t value[PR_MAX_NUM_BYTES];
  char unit[PR_UNIT_NAME_LIMIT];
  struct pr_row_per_t *next; // linked list
} pr_row_per;

typedef struct {
  pr_health_state health;
  time_t heartbeat;
  uint64_t uptime;
  uint16_t peripheral_count;
  pr_row_per *first_row; // (NULL) if there are no peripherals
} pr_status_sys_t;

// writes (and blocks until) received status into variable
pr_ret_t pr_status_sys(pr_status_sys_t *status);
pr_ret_t pr_status_per(const char *peripheral, pr_status_per_t *status);
// size of string parameters are expected to be at least
// PR_VERSION_STRING_BUFFER
pr_ret_t pr_version(uint8_t *fw, uint8_t *protocol, char *fw_str,
                    char *protocol_str);
void pr_watch(void);

#endif
