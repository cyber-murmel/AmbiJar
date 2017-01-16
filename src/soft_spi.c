#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "soft_spi.h"

#define DDR_(X) DDR##X
#define DDR(X) DDR_(X)
#define PORT_(X) PORT##X
#define PORT(X) PORT_(X)

#ifndef CLK_REG
#define CLK_REG B
#endif

#ifndef DAT_REG
#define DAT_REG B
#endif

#ifndef CLK_PIN
#define CLK_PIN (4)
#endif

#ifndef DAT_PIN
#define DAT_PIN (3)
#endif

#define CLK_DOWN PORT(CLK_REG) &= ~(1<<CLK_PIN);
#define CLK_UP   PORT(CLK_REG) |= 1<<CLK_PIN;

#define DAT_DOWN PORT(DAT_REG) &= ~(1<<DAT_PIN);
#define DAT_UP   PORT(DAT_REG) |= 1<<DAT_PIN;

inline void spi_init() {
    // set both low
    PORT(CLK_REG) &= ~(1<<CLK_PIN);
    PORT(DAT_REG) &= ~(1<<DAT_PIN);
    // set both as output
    DDR(CLK_REG) |= (1<<CLK_PIN);
    DDR(DAT_REG) |= (1<<DAT_PIN);
}

inline void spi_send_bit(uint8_t bit) {
    CLK_DOWN
    if(bit){
        DAT_UP
    }
    _delay_us(1);
    CLK_UP
    _delay_us(1);
    CLK_DOWN
    DAT_DOWN
}

inline void spi_send_byte(uint8_t byte) {
    for(int8_t i = 7; i >= 0; i--){
        spi_send_bit((byte & (1<<i))>>i);
    }
}

inline void spi_send_buf(void *buf, uint16_t size) {
    uint8_t *byte_buf = (void*)buf;
    for(uint16_t i = 0; i < size; i++){
       spi_send_byte(byte_buf[i]);
    }
    _delay_us(500);
}

