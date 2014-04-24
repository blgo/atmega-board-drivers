#include <avr/interrupt.h>
#include <avr/io.h>

unsigned int times = 0;

/**
* Interrupt Service Routine (ISR) for handling
* INT4 interrupts.
* Toggles PH0 for every interrupt
*/
ISR(INT4_vect) 
{
    times++;
    PORTH = times;
    PORTB ^= _BV(PB5);        
} 

/**
* Initialize the asyncron port pin toggler.
* Setup INT4 (PE4) to interrupt on all level changes.
* Setup PE4 to input.
* Setup PH0 to output
*/
void init_port_toggler( void ) {
    // Set PE4 (INT4) to input
    DDRE &= ~_BV(DDE4);

    DDRH |= 0xFF;
    DDRB |= _BV(DDB5);

    // Set INT4 to interrupt on every level change
    EICRB |= _BV(ISC40);

    // Enable INT4 in the Externalinterrupt register
    EIMSK |= _BV(INT4);
}


void main(void)
{
    init_port_toggler();

    sei();

    while(1) {
    }
}
