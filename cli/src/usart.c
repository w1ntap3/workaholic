#include "usart.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

void usart_read(uint8_t *byte_buffer, const int buf_size) {
  int file_d = open(USART_FILESTREAM, O_RDONLY);
  ssize_t read_n = read(file_d, byte_buffer, buf_size);
  for (int i = 0; i < buf_size; i++) {
    printf("%d", *byte_buffer + i);
  }
  return;
}
