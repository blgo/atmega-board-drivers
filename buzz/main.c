#include <avr/io.h>

void init_buzz(void)
{
    DDRB |= 0xFF;

    PORTB |= _BV(PB5);
}

void main(void)
{
    init_buzz();
    while(1) {
        

    }
}
