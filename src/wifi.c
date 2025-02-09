#include "wifi.h"

void init_wifi() {
    usart_init(USART1);

    // Send test AT command to ESP8266
    usart_send_string(USART1, "AT+RST\r\n");
    char buffer[100];

    int length = usart_receive(USART1, buffer, 100);
    for(int i = 0;i < 100; i++) {
        usart_send_char(USART2, buffer[i]);
    }
}