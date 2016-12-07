void spi_init();

void spi_send_bit(uint8_t bit);

void spi_send_byte(uint8_t byte);

void spi_send_buf(void *buf, uint16_t size);
