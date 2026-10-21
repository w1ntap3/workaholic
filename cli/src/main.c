#include "protocol.h"
#include <fcntl.h>
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// DEFINES (start)
typedef enum {
  PROTOCOL_VERSION_MATCH = 0,
  PROTOCOL_VERSION_FAIL,
  PROTOCOL_VERSION_CLASH
} cli_ret_t;

// DEFINES (end)

// PF (start)
int cli_health(int argc);

int cli_version(void);
int get_firmware_version(char *firmware_version, int buf);
cli_ret_t get_protocol_version(char *protocol_version, int buf);

static inline void help_util(void);

int cli_status(int argc, char *argv[]);
int cli_update(int argc, char *argv[]);
int cli_watch(int argc, char *argv[]);
// PF (end)

// PV (start)
static const struct {
  const char *cmd_name;
  int (*cmd_handler)(int, char **);
} cmd_table[] = {
    {"status", cli_status}, {"update", cli_update}, {"watch", cli_watch}};
// PV (end)

// static const struct commands
int main(int argc, char *argv[]) {
  // Not enough arguments
  if (argc < 2) {
    fprintf(stderr, "Usage: workaholic --help\n");
    return 1;
  }
  // Global help
  if ((strcmp(argv[1], "--help") == 0) || (strcmp(argv[1], "help") == 0)) {
    help_util();
    return 0;
  }
  if ((strcmp(argv[1], "--version") == 0) ||
      (strcmp(argv[1], "version") == 0)) {
    return pr_version();
  }
  if (strcmp(argv[1], "health") == 0) {
    return cli_health(argc);
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

  fprintf(stderr, "Error: No such command as \"%s\"\n", argv[1]);
  return 1;
}

int cli_health(int argc) {
  if (argc >= 0) {
    printf("workaholic cli is working.\n");
    return 0;
  } else {
    printf("what...");
    return 1;
  }
}

int cli_version(void) {
  int version_ret = 0;
  printf("CLI version: %s\n", CLI_VERSION);

  // Request the firmware version from the MCU
  char firmware_version[PR_VERSION_STRING_BUFFER];
  int ret =
      get_firmware_version(firmware_version, (int)PR_VERSION_STRING_BUFFER);
  if (ret == 0) {
    printf("Firmware version: %s\n", firmware_version);
  } else {
    printf("Firmware version: Failed to retrieve.\n");
    version_ret++;
  }

  // Compare the assumed protocol version of the CLI vs MCU's
  char protocol_version[PR_VERSION_STRING_BUFFER];
  ret = get_protocol_version(protocol_version, PR_VERSION_STRING_BUFFER);
  switch (ret) {
  case PROTOCOL_VERSION_MATCH:
    printf("Protocol version: %s\n", protocol_version);
    break;
  case PROTOCOL_VERSION_FAIL:
    printf("Protocol version: Failed to retrieve.\n");
    version_ret++;
    break;
  case PROTOCOL_VERSION_CLASH:
    printf("Protocol version: Clash occured. CLI claims \"%s\" while MCU "
           "claims \"%s\"\n",
           CLI_PROTOCOL_VERSION, protocol_version);
    version_ret++;
    break;
  }

  return version_ret;
}

// TODO: implement firmware version request
int get_firmware_version(char *firmware_version, int buf) {
  (void)firmware_version;
  (void)buf;
  return 1;
}

// TODO: implement protocol version request + comparison
cli_ret_t get_protocol_version(char *protocol_version, int buf) {
  (void)protocol_version;
  (void)buf;
  return PROTOCOL_VERSION_FAIL;
}

int cli_status(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  int file_d = open(USART_FILESTREAM, O_RDONLY);
  char byte_buffer[128];
  memset(byte_buffer, 0, 128);
  read(file_d, byte_buffer, 128);

  printf("{");

  for (int i = 0; i < 128; i++) {
    printf("%c,", byte_buffer[i]);
  }
  printf("}\n");
  return 0;
}

int cli_update(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  return 0;
}

int cli_watch(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  return 0;
}

static inline void help_util(void) {
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
}
