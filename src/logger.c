#include "logger.h"

void init_logger() {
    usart_init(USART2);
}

void LOG(const char* msg) {
    usart_send_string(USART2, msg);
}