#include <avr/io.h>
#include <util/delay.h>

#include "drivers.h"

/*
 * ================
 * Helper functions
 * ================
 */

/*
 * Keyboard
 */

void init_keyboard(void)
{
    DDRJ = 0xF0;
    PORTJ = 0x0F;
}

int read_keyboard_column(void)
{
    init_keyboard();
    int col = KBD_INPUT;

    switch (col) {
        case 0x07:
            return 0;
        case 0x0B:
            return 1;
        case 0x0D:
            return 2;
        case 0x0E:
            return 3;
        default:
            return -1;
    }
}

int read_keyboard_row(void)
{
    init_keyboard();
    int is_row, row;

    // row as input
    PORTJ = 0x0F;

    for(row = 0; row < 4; row++) {
        // put row bit to LOW
        KBD_OUTPUT = ~(0x01 << (row + 4)) & 0xF0;

        is_row = KBD_INPUT & 0x0F;
        if (is_row == 0x0F) {
            // not pressed
            continue;
        } else {
            // pressed; return row number
            return 3 - row;
        }
    }

    // column as input
    PORTJ = 0x0F;

    return -1;
}

/*
 * SPI with temperature sensor.
 */

void enable_temperature_sensor(void)
{
    // PK5 in PORTK Is the EC (enabler), it must be set high when we want to 
    // read/write
    DDRK = _BV(DDK5);		
}

void spi_transmit(uint8_t data)
{
    SPDR = data;                             
    // Wait for transmission to complete 
    while (!(SPSR & (1<<SPIF)));
}

uint8_t spi_receive(void)
{
    // dummy data for starting the clock
    SPDR = 0x00; 					
    
    // Wait for reception to complete
    while (!(SPSR & (1<<SPIF)));
    
    return SPDR;
}

void init_spi_temperature_sensor(void)
{
    // SPI pins on port B: MISO, MOSI, SCK, SS 
    DDRB = _BV(DDB2) | _BV(DDB1) | _BV(DDB0); // MISO as pull up
    PORTB |= _BV(PB3) | _BV(PB0);

    // TODO: document missing config
    // SPI configuration:
    // - master
    // - msb first 
    // - load when clock is falling
    // - sample when clock is raising
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0) | _BV(CPOL) | _BV(CPHA);
}

void init_temperature_sensor_oneshot(void){			
    // use every time that you want to read the temperature; the sensor goes 
    // iddle when finish the measurement
    init_spi_temperature_sensor();
    PORTK |= _BV(PK5); 
    spi_transmit(0x80);  // write mode
    spi_transmit(0x10);  // one shot mode writing on the register memory sensor
    PORTK &= ~_BV(PK5);
}

/*
 * SPI 7 segment display
 */

/*
 * Initialize SPI to write to the 7 segments display.
 */
void init_spi_7seg(void)
{
    // NOTE:
    //  the clock is divided by 128
    
    // initialize 7 segment selector
    DDRL = 0x0F;
    DDRK  = _BV(DDK3);
   
    // SPI pins on port B: MOSI, SCK, SS
    DDRB = _BV(DDB2) | _BV(DDB1) | _BV(DDB0);
    // - pull up resistor enabled
    // - works as Master reset "deny" (_MR)
    // -_SS as HI level in SPI
    PORTB = _BV(PB3) | _BV(PB0); 					

    // TODO: document missing config
    // SPI configuration:
    // - master
    // - msb first 
    // - load when clock is falling
    // - sample when clock is raising
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(DORD) | _BV(CPHA) | _BV(CPOL) |_BV(SPR1) | _BV(SPR0);
}

/* 
 * =================
 * 7 segment display
 * =================
 */
void display_7seg(int pos, uint8_t data)
{
    init_spi_7seg();
    
    // clear 7seg
    SPDR = 0;
    while (!(SPSR & (1<<SPIF)));
    PORTL = _BV(pos);
    PORTK &= ~_BV(PK3);
    PORTK |= _BV(PK3);
    
    // Start transmission
    SPDR = data;

    // Wait for transmission complete
    while (!(SPSR & (1<<SPIF)));
    PORTL = _BV(pos);
    PORTK &= ~_BV(PK3);
    PORTK |= _BV(PK3);
}

void clear_7seg(void)
{
    int i;
    for (i = 0; i < MAX_7SEG; i++) {
        display_7seg(i, 0);
    }
}

/* 
 * ========
 * Keyboard
 * ========
 */

void read_keyboard_coords(int* row, int* column)
{
    int row_value = -1;
    int col_value = -1;

    while (row_value < 0 || row_value > 3 || col_value < 0 || col_value > 3) {
        row_value = read_keyboard_row();
        col_value = read_keyboard_column();
    }

    // key has been pressed
    *row = row_value;
    *column = col_value;

    // wait until key release
    while (row_value == *row && col_value == *column) {
        row_value = read_keyboard_row();
        col_value = read_keyboard_column();
    }

    _delay_ms(300);
}

/* 
 * ==================
 * Temperature sensor
 * ==================
 */

void init_temperature_sensor(void)
{
    init_spi_temperature_sensor();
}

int read_temperature(void)
{
    init_spi_temperature_sensor();
    init_temperature_sensor_oneshot();
    enable_temperature_sensor();

    // read LSB
    PORTK |= _BV(PK5);
    spi_transmit(0x01);
    uint8_t LSB = spi_receive();
    PORTK &= ~_BV(PK5);

    // read MSB
    PORTK |= _BV(PK5);    
    spi_transmit(0x02);
    uint8_t MSB = spi_receive();
    PORTK &= ~_BV(PK5);

    return (int) ((MSB << 2) | (LSB >> 6));
}
