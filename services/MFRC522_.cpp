
#include "MFRC522_.h"

//Set a GPIO pin
void digitalWrite(uint PIN, bool STATE){
    gpio_put(PIN, STATE);
}

//Read a GPIO pin
bool digitalRead(uint PIN){
    return gpio_get(PIN);
}

//Current absolute time relative to boot time in milliseconds
uint32_t millis(){
    return to_ms_since_boot(get_absolute_time());
}

//Do nothing
void yield(){
    return;
}

//Do nothing wait in microseconds
void delayMicroseconds(uint32_t delayTime){
    busy_wait_us(delayTime);
}

//Do nothing wait in milliseconds
void delay(uint32_t delayTime){
    busy_wait_ms(delayTime);
}

//Set the output pin mode
void pinMode(uint PIN, bool MODE){
    gpio_init(PIN);
    gpio_set_dir(PIN, MODE);
}

//Transfer SPI Data
uint8_t spi::transfer(uint8_t data){
    uint8_t read;
    spi_write_read_blocking(RFID_SPI, &data, &read, 1);
    return read;
}

//Initialize the Spi system
void spi::spiInit(uint baudrate, uint SCK_PIN, uint MOSI_PIN, uint MISO_PIN){
    spi_init(RFID_SPI, baudrate);
    gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MISO_PIN, GPIO_FUNC_SPI);
}