#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "drivers.h"

#define LEDS              PORTH
#define LEDS_CONTROL      DDRH

#define STATE_STANDBY     0
#define STATE_TEMPERATURE 1
#define STATE_INPUT       2
#define STATE_SCORE       3

int state = STATE_STANDBY;
int timer_is_counting = 0;

/*
 * ================
 * Helper functions
 * ================
 */

/*
 * Initialize LEDs in PORT H.
 */
void init_leds(void)
{
    LEDS_CONTROL = 0xFF;
    LEDS = 0x00;
}

/*
 * Initialize timer.
 */
void init_timer(void){
	// Normal mode with default values
	// Set preescaler at clk/256
	TCCR1B = _BV(CS12) & ~_BV(CS11) & ~_BV(CS10);
	// Compare match on TOP
	OCR1A = 0xFFFF;
	// when OCIEOA is HIGH: Interrupt enable TIMER1_COMPA_vect
	TIMSK1 |= _BV(OCIE1A);
}

ISR(TIMER1_COMPA_vect){
	if (state == STATE_TEMPERATURE) {
        if (timer_is_counting) {
            state = STATE_INPUT;
            timer_is_counting = 0;
        } else {
            timer_is_counting = 1;
        }
    } else if (state == STATE_SCORE) {
        if (timer_is_counting) {
            state = STATE_STANDBY;
            timer_is_counting = 0;
        } else {
            timer_is_counting = 1;
        }
    }
}

/*
 * Return the number of 1s in `byte`.
 */
int number_of_1s(uint8_t byte)
{
    int count;

    for (count = 0; byte; count++) {
        // clear the LSB that is set to 1
        byte &= byte - 1; 
    }
    return count;
}

/*
 * Initialize Key1.
 */
void init_key(void)
{
    // Set PORTD4 (INT4) to input
    DDRE &= ~_BV(DDE4);

    DDRB |= _BV(DDB5);

    // set INT4 to interrupt on every level change
    EICRB |= _BV(ISC40);

    // enable INT4 in the ExternalInterrupt register
    EIMSK |= _BV(INT4);
}

ISR(INT4_vect)
{
    if (state == STATE_STANDBY) {
        state = STATE_TEMPERATURE;
    }
    PORTB ^= _BV(PB5);
}

/*
 * Write the word `HEJ` in the 7 segment display.
 */
void say_hello(void)
{
    display_7seg(2, 0x6E);
    display_7seg(1, 0x9E);
    display_7seg(0, 0x78);
}

/*
 * ============
 * Main program
 * ============
 */


int main(void)
{
    int temperature;
    int inserted_value;
    int have_read_temperature = 0;

    sei();

    init_leds();
    init_key();
    init_timer();
    
    // hex values for the 7 segment display from 0 to 9
    int values[10] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE,
                      0xE0, 0xFE, 0xF5};
                              

    while (1) {
        if (state == STATE_STANDBY) {
            // standby, wait the user for pressing key1
            say_hello();
            LEDS = 0;
        } else if (state == STATE_TEMPERATURE) {
            // read temperature, display in LEDs and change to state 2
            clear_7seg();
            // ensure that we read the temperature only once
            if (!have_read_temperature) {
                temperature = read_temperature();
                have_read_temperature = 1;
            }
            LEDS = temperature;
        } else if (state == STATE_INPUT) {
            // reset `have_read_temperature`
            have_read_temperature = 0;
            clear_7seg();
            int* row = malloc(sizeof(int));
            int* col = malloc(sizeof(int));

            // read 8 keyboard presses and store them in `inserted_value`
            int keys_left = 8;
            inserted_value = 0;

            while (keys_left != 0) {
                // show inserted value as is being inserted
                LEDS = inserted_value;
                display_7seg(0, values[keys_left]);
                read_keyboard_coords(row, col);
                if (*row == 0 && (*col == 0 || *col == 1)) {
                    // 0 or 1 pressed
                    inserted_value |= *col << (keys_left -1);
                    keys_left--;
                }
            }
            free(row);
            free(col);

            state = STATE_SCORE;
        } else if (state == STATE_SCORE) {
            // show guess number in 7seg
            int guessed_bits = ~(temperature ^ inserted_value);
            int guesses = number_of_1s(guessed_bits);
            display_7seg(0, values[guesses]);
            // show original value in LEDs
            LEDS = temperature;
        }
    }

    return 0;
}
