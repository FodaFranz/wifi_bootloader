#pragma once

#include <stdint.h>
#include "stm32f4xx.h"

#define SYS_FREQ 16000000
#define BAUDRATE 115200

void usart_set_baudrate(USART_TypeDef* USARTx, uint32_t periph_clock, uint32_t baudrate);
// TODO: maybe make this generic
void usart_init(USART_TypeDef* USARTx);
void usart_send_char(USART_TypeDef* USARTx, char c);
void usart_send_string(USART_TypeDef* USARTx, const char* str);