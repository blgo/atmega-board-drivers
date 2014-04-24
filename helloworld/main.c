#include <avr/io.h>

void init_buzz(void)
{
    DDRH = 0xFF;
    PORTH |= _BV(6);
}

void main(void)
{
    init_buzz();
    while(1) {
   
    }
}
