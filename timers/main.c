#include <avr/interrupt.h>
#include <avr/io.h>

void init_timer(void)
{
    // CTC Mode

    // Setup OC4A to toggle on compare match
    TCCR4A |= _BV(COM4A0);
    // Setup Timer 4 prescaler to clk/8
    TCCR4B |= _BV(CS41) | _BV(WGM12);

    // Counter
    OCR4A = 0xA000;

    // Setup PH3 to output
    DDRH |= _BV(DDH3);
}

void main(void)
{
    init_timer();

    while(1) {
    }
}
