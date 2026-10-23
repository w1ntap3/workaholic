#include "protocol.h"
#include <fcntl.h>
#include <getopt.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// PF (start)
int cli_status(int argc, char *argv[]);
int cli_update(int argc, char *argv[]);
int cli_watch(int argc, char *argv[]);
int cli_version(int argc, char *argv[]);
int cli_help(int argc, char *argv[]);
// PF (end)

// PV (start)
static const struct {
  const char *cmd_name;
  int (*cmd_handler)(int, char **);
} cmd_table[] = {{"status", cli_status},
                 {"update", cli_update},
                 {"watch", cli_watch},
                 {"version", cli_version},
                 {"help", cli_help}};
// PV (end)

// static const struct commands
int main(int argc, char *argv[]) {
  // Not enough arguments
  if (argc < 2) {
    fprintf(stderr, "Usage: workaholic help\n");
    return 1;
  }
  // Global help
  if (strcmp(argv[1], "--help") == 0) {
    return cli_help(argc, argv);
  }
  if (strcmp(argv[1], "--version") == 0) {
    return cli_version(argc, argv);
  }

  const char *cmd = argv[1];
  const size_t table_entries = sizeof(cmd_table) / sizeof(cmd_table[0]);
  for (size_t i = 0; i < table_entries; i++) {
    if (strcmp(cmd, cmd_table[i].cmd_name) == 0) {
      // Do not pass the program and subcommands (therefore
      // argv + 2) to the subcommand handlers since these are
      // trivial anyways
      int result = cmd_table[i].cmd_handler(argc - 2, argv + 2);
      return result;
    }
  }

  fprintf(stderr, "Error: No such subcommand as \"%s\"\n", argv[1]);
  return 1;
}

// TODO: fix printfs with buffer writes and proper error handling
static int get_health_str(const pr_health_state health, const uint8_t is_sys,
                          char *health_str, size_t str_size) {
  const char *str;
  switch (health) {
  case PR_STATE_HEALTHY:
    str = "Healthy";
    break;
  case PR_STATE_DISCONNECTED:
    str = "Disconnected";
    break;
  case PR_STATE_DEAD:
    str = "Dead";
    break;
  case PR_STATE_WEAK:
    str = "Weak";
    break;
  case PR_STATE_BELOW_RANGE:
    str = is_sys ? "Buggy" : "Below healthy range";
    break;
  case PR_STATE_ABOVE_RANGE:
    str = is_sys ? "Buggy" : "Above healthy range";
    break;
  default:
    if (str_size > 0) {
      health_str[0] = '\0';
    }
    return -1;
  }
  int result = snprintf(health_str, str_size, "%s", str);
  if (result < 0 || (size_t)result >= str_size)
    return -1;

  return (is_sys &&
          (health == PR_STATE_BELOW_RANGE || health == PR_STATE_ABOVE_RANGE));
}

// TODO: fix printfs with buffer writes and proper error handling
static int get_heartbeat_str(time_t heartbeat, time_t cur_time) {
  if (heartbeat > cur_time) {
    printf("Bug occured, heartbeat cannot have happened in future");
    return 1;
  }
  const uint64_t sec_ago = (uint64_t)(cur_time - heartbeat);
  printf("%lu seconds ago", sec_ago);
  return 0;
}

static int render_table(pr_row_per *first_row, int peripheral_count) {
  (void)first_row;
  (void)peripheral_count;
  printf("ID NAME HEALTH VALUE UNIT\n");
  return 0;
}

int cli_status(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  if (!argv[0]) {
    pr_status_sys_t sys_status;
    pr_status_sys(&sys_status);

    // int ret;
    printf("Connection: \n");
    // get_health_str(sys_status.health, 1);

    struct timespec cur_time;
    clock_gettime(CLOCK_REALTIME, &cur_time);

    printf("Last MCU heartbeat: \n");
    // get_heartbeat_str(sys_status.heartbeat, cur_time.tv_sec);
    printf("Uptime: \n");

    printf("Peripheral table:\n");
    render_table(sys_status.first_row, sys_status.peripheral_count);
  }
  return 0;
}

int cli_update(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  return 1;
}

int cli_watch(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  // ^C to stop
  pr_watch();
  // shouldnt even reach this point so if it does, something is wrong
  return 1;
}

int cli_version(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  uint8_t fw, protocol;
  char fw_str[PR_VERSION_STRING_BUFFER], protocol_str[PR_VERSION_STRING_BUFFER];
  pr_version(&fw, &protocol, fw_str, protocol_str);

  return 1;
}
int cli_help(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  printf("Usage: workaholic <command> [options]\n"
         "\n"
         "  status [peripheral]                        Show connection, "
         "uptime and peripheral health;\n"
         "                                             or details for a single "
         "peripheral\n"
         "  update <peripheral> --<parameter> <value>  Update a peripheral's "
         "settings (e.g. --name, --min, --max)\n"
         "  watch                                      Stream peripheral "
         "events until interrupted\n"
         "  --version                                  Show CLI, firmware and "
         "protocol versions\n"
         "  --help                                     Show this message\n"
         "\n"
         "Examples:\n"
         "  workaholic status\n"
         "  workaholic status temp\n"
         "  workaholic update temp --min 0 --max 100 --name \"Temperature "
         "Sensor\"\n"
         "  workaholic watch\n");
  return 0;
}
