#include <avr/io.h>

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

void toggle_shcp(void){
	selectseg();
		PORTB ^= _BV(PB1);	
		PORTB ^= _BV(PB1);

}
void toggle_stcp(void){
 		PORTK ^= _BV(PK3);
		PORTK ^= _BV(PK3);
}


void writedec(char a[8]){ //input number(come from the keyboard)
	unsigned int i;
	for(i=0;i<=8;i++){
		if (a[i]== 1) PORTB |= _BV(PB2);
		else if(a[i] == 0) PORTB &= ~_BV(PB2);
		else error();
	toggle_shcp();
	}
	toggle_stcp();
	
}



void selectseg(void){
	PORTL = _BV(PL0);

}



void main(void){
    init_reg();
    
 	while(1) {
	toggle_shcp();
	write();
	toggle_stcp();
    }
}
