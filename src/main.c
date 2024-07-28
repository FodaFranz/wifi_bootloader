#include <stdint.h>
#include "stm32f4xx.h"

#define LED_PIN 5

#define FLASH_KEY1 0x45670123
#define FLASH_KEY2 0xCDEF89AB

#include "test_binary.h"
#include "logger.h"

extern int __approm_start__;

__attribute__((noreturn)) void led_blinky_is_alive() {
    RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOAEN_Pos);

    // do two dummy reads after enabling the peripheral clock, as per the errata
    volatile uint32_t dummy;
    dummy = RCC->AHB1ENR;
    dummy = RCC->AHB1ENR;

    GPIOA->MODER |= (1 << GPIO_MODER_MODER5_Pos);
    
    while(1)
    {
        GPIOA->ODR ^= (1 << LED_PIN);
        for (uint32_t i = 0; i < 100000; i++);
    }
}

void unlock_flash() {
    // Allow write to flash, unlock the flash control register (p. 48)
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
}

void write_byte_to_flash(uint8_t* address, uint8_t value) {
    // Configure parallelism
    FLASH->CR &= ~(FLASH_CR_PSIZE_0);
    FLASH->CR &= ~(FLASH_CR_PSIZE_1);

    if (!(FLASH->SR & FLASH_SR_BSY)) {
        FLASH->CR |= FLASH_CR_PG;
        *address = value;

        while((FLASH->SR & FLASH_SR_BSY)) {
        }

        FLASH->CR &= ~(FLASH_CR_PG);
    }
}   

static void start_app(uint32_t pc, uint32_t sp) {
    __asm("           \n\
          msr msp, r1 /* load r1 into MSP */\n\
          bx r0       /* branch to the address at r0 */\n\
    ");
}

int main() {
    unlock_flash();

    uint8_t* addr = (uint8_t*) 0x8004000;
    for(int i = 0;i < test_binary_len; i++) {
        write_byte_to_flash(addr, test_binary[i]);
        addr += 1;
    }

    init_logger();
    LOG("Start Bootrom");

    // TODO: do not hardcode address of application
    uint32_t *app_code = (uint32_t *) 0x8004000;
    uint32_t app_sp = app_code[0];
    uint32_t app_start = app_code[1];

    start_app(app_start, app_sp);

    return 0;
}