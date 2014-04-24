#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>

#define KBD_INPUT   PINJ
#define KBD_OUTPUT  PORTJ


void init_keyboard(void)
{
    // PORTJ
    DDRJ = 0xF0;
    PORTJ = 0x0F;

    // PORTH as output
    DDRH = 0xFF;
    PORTH = 0x00;
}

char get_char(void)
{
    unsigned int col = read_column();
    unsigned int row = read_row();
    unsigned int coords = row | col;

    switch (coords)
    {
        case 0x77:
            PORTH = 0;
            return '0';
            break;
        case 0x7B:
            PORTH = 1;
            return '1';
            break;
        case 0x7D:
            PORTH = 2;
            return '2';
            break;
        case 0x7E:
            PORTH = 3;
            return '3';
            break;
        case 0xB7:
            PORTH = 4;
            return '4';
            break;
        case 0xBB:
            PORTH = 5;
            return '5';
            break;
        case 0xBD:
            PORTH = 6;
            return '6';
            break;
        case 0xBE:
            PORTH = 7;
            return '7';
            break;
        case 0xD7:
            PORTH = 8;
            return '8';
            break;
        case 0xDB:
            PORTH = 9;
            return '9';
            break;
        case 0xDD:
            PORTH = 10;
            return 'A';
            break;
        case 0xDE:
            PORTH = 11;
            return 'B';
            break;
        case 0xE7:
            PORTH = 12;
            return 'C';
            break;
        case 0xEB:
            PORTH = 13;
            return 'D';
            break;
        case 0xED:
            PORTH = 14;
            return 'E';
            break;
        case 0xEE:
            PORTH = 15;
            return 'F';
            break;
        default:
            PORTH = 0;
            return ' ';
    }
}

int read_column(void)
{
    unsigned int column = KBD_INPUT;
    return column;
}

int read_row(void)
{
    int i;
    unsigned int row;

    for(i = 0; i < 4; i++) {
        // put i-th row bit to LOW
        PORTJ = ~(0x01 << i + 4) & 0xF0;

        row = KBD_INPUT & 0x0F;
        if (row == 0x0F) {
            // not pressed
            continue;
        } else {
            // pressed
            PORTJ = 0x0F;
            return ~(0x01 << i + 4) & 0xF0;
        }
    }
}

int main(void)
{
    unsigned int row, column;

    init_keyboard();

    while(1)
    {
        get_char();
    }

    return 0;
}
