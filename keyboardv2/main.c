#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <string.h>

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
            return "11111100";
            break;
        case 0x7B:
            PORTH = 1;
            return "01100000";
            break;
        case 0x7D:
            PORTH = 2;
            return "11011010";
            break;
        case 0x7E:
            PORTH = 3;
            return "11110010";
            break;
        case 0xB7:
            PORTH = 4;
            return "01100110";
            break;
        case 0xBB:
            PORTH = 5;
            return "10110110";
            break;
        case 0xBD:
            PORTH = 6;
            return "10111110";
            break;
        case 0xBE:
            PORTH = 7;
            return "11100000";
            break;
        case 0xD7:
            PORTH = 8;
            return "11111110";
            break;
        case 0xDB:
            PORTH = 9;
            return "11110110";
            break;
       /* case 0xDD:
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
	*/
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

void error(void){
			DDRH |= _BV(DDH7);
			PORTH |= _BV(PH7);
			//ERROR
			}

void init_reg(void){
	DDRB |= _BV(DDB1)  |  _BV(DDB2) | _BV(DDB3);
	DDRK = _BV(DDK3);
	DDRL |= _BV(DDL1) | _BV(DDL2) | _BV(DDL3) | _BV(DDL0) ;
	PORTB |= _BV(PB1);
	PORTK &= ~_BV(PK3);
	PORTB = _BV(PB3);
}
void toggle_shcp(void){
		PORTB ^= _BV(PB1);	
		PORTB ^= _BV(PB1);

}
 void toggle_stcp(void){
 		PORTK ^= _BV(PK3);
		PORTK ^= _BV(PK3);
}

void selectseg(void){
	PORTL = _BV(PL0);
}
/*
void writedec(void){ //input number(come from the keyboard)
	unsigned int i;
	for(i=0;i<=7;i++){
		if (a[i]== 1) PORTB |= _BV(PB2);
		else if(a[i]== 0) PORTB &= ~_BV(PB2);
		else error();
	toggle_shcp();
	}
	toggle_stcp();
}
*/



void main(void){
    init_reg();
    unsigned int row, column;
    init_keyboard();
 	while(1) {
	selectseg();
	writedec();
    }
}