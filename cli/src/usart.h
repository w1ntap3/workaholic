#ifndef USART_H
#define USART_H

#include <stddef.h>
#include <stdint.h>

#define USART_FILESTREAM "/dev/ttyACM0"

typedef enum { US_OK = 0, US_FAIL } us_ret_t;

us_ret_t us_rx(void *buf, const size_t buf_size);
us_ret_t us_tx(void *buf, const size_t buf_size);

#endif
