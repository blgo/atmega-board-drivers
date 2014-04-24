#include <avr/interrupt.h>
#include <avr/io.h>

void init_pwm(void)
{
    // Fast PWM Mode 10-bits

    // Setup OC4A to toggle on compare match
    TCCR4A |= _BV(COM4A0);

    TCCR4B |= _BV(WGM12) | _BV(WGM11) | _BV(WGM10);

    // TOP
    ICR4 = 0xFFFF;

    // Counter
    OCR4A = 0xFE00;

    // Setup PH3 to output
    DDRH |= _BV(DDH3);
}


void main(void)
{
    init_pwm();

    while(1) {
    }
}
