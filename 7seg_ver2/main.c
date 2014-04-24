#include <avr/io.h>

#define STCP_DDR    DDRK
#define SHCP_DDR    DDRB
#define DS_DDR      DDRB
#define MR_DDR      DDRB

#define STCP_PORT   PORTK
#define SHCP_PORT   PORTB
#define DS_PORT     PORTB
#define MR_PORT     PORTB

#define STCP_PIN    PK3
#define SHCP_PIN    PB1
#define DS_PIN      PB2
#define MR_PIN      PB3

/*
 * Initialize 74HC595.
 *
 * */
void init_7seg_display(void)
{
    // Storage clock
    STCP_DDR |= _BV(STCP_PIN);
    // Shift clock
    SHCP_DDR |= _BV(SHCP_PIN);
    // serial data input    
    DS_DDR |= _BV(DS_PIN);
    // Master reset
    MR_DDR |= _BV(MR_PIN);

    DDRL = 0xFF;
    PORTL = 0xFF;

    // SHCP LOW
    SHCP_PORT &= ~_BV(SHCP_PIN);

    // STCP HIGH
    STCP_PORT |= _BV(STCP_PIN);

    // MR HIGH
    MR_PORT |= _BV(MR_PIN);
}

void toggle_shcp(void)
{
    SHCP_PORT &= ~_BV(SHCP_PIN);
    SHCP_PORT |= _BV(SHCP_PIN);
}

void toggle_stcp(void)
{
    STCP_PORT &= ~_BV(STCP_PIN);
    STCP_PORT |= _BV(STCP_PIN);
}

void store(void)
{
}

int main(void)
{
    init_7seg_display();
    
    while (1) {
        int i;
        for (i=0; i<8; i++) {
            DS_PORT |= _BV(DS_PIN);
            toggle_shcp();
        }
        toggle_stcp();
    }
}