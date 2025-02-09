#include "uart.h"

static uint16_t compute_usart_baudrate(uint32_t periph_clock, uint32_t baudrate) {
  return ((periph_clock + (baudrate/2U))/baudrate);
}

void usart_init(USART_TypeDef* USARTx) {
  if (USART2 == USARTx) {
    RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOAEN_Pos);
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Set GPIO pins to UART mode RX = PA3, TX = PA2, alternate function 7
    GPIOA->MODER |= GPIO_MODER_MODER3_1;
    GPIOA->MODER |= GPIO_MODER_MODER2_1;
    GPIOA->AFR[0] |= (7U << GPIO_AFRL_AFSEL2_Pos);
    GPIOA->AFR[0] |= (7U << GPIO_AFRL_AFSEL3_Pos);
  }
  else if (USART1 == USARTx) {
    RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOAEN_Pos);
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // RX = PA10, TX = PA9
    GPIOA->MODER |= GPIO_MODER_MODER9_1;
    GPIOA->MODER |= GPIO_MODER_MODER10_1;
    GPIOA->AFR[1] |= (7U << GPIO_AFRL_AFSEL1_Pos);
    GPIOA->AFR[1] |= (7U << GPIO_AFRL_AFSEL2_Pos);
  }

  // Configure baudrate
  usart_set_baudrate(USARTx, SYS_FREQ, BAUDRATE);

  // Enable UART module in transceiver mode
  USARTx->CR1 = USART_CR1_TE | USART_CR1_RE;

  // Enable UART
  USARTx->CR1 |= USART_CR1_UE;
}

void usart_set_baudrate(USART_TypeDef* USARTx, uint32_t periph_clock, uint32_t baudrate) {
  USARTx->BRR = compute_usart_baudrate(periph_clock, baudrate);
}

void usart_send_char(USART_TypeDef* USARTx, char c) {
  while(!(USARTx->SR & USART_SR_TXE)) {}

  USARTx->DR = c & 0xFF;
}

void usart_send_string(USART_TypeDef* USARTx, const char* str) {
  while(*str != 0) {
      usart_send_char(USARTx, *str);
      str++;
  }
}

char usart_receive_char(USART_TypeDef* USARTx) {
  while (!(USARTx->SR & USART_SR_RXNE)) {}

  return USARTx->DR;
}

int usart_receive(USART_TypeDef* USARTx, char* buffer, int buffer_length) {
  for (int i = 0;i < buffer_length; i++) {
    buffer[i] = usart_receive_char(USARTx);
    if (buffer[i] == '\n') {
      // TODO: might overwrite memory, additional checks
      buffer[i+1] = 0;
      return (i+1);
    }
  }

  // TODO: proper way to indicate an error
  return 0;
}