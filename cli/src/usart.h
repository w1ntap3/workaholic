#ifndef USART_H
#define USART_H

#include <stdint.h>

#define USART_FILESTREAM "/dev/ttyACM0"

void usart_init();
void usart_write();
void usart_read(uint8_t *byte_buffer, const int buf_size);

#endif
