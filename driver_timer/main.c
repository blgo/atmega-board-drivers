#include <avr/interrupt.h>
#include <avr/io.h>

int i=0;
void init_timer(void){
	//Normal mode with default values
	//Set preescaler at clk/256
	TCCR1B = _BV(CS12) & ~_BV(CS11) & ~_BV(CS10);
	//Compare match on TOP
	OCR1A = 0xFFFF;
	//when OCIEOA is HI: Interrupt enable TIMER1_COMPA_vect
	TIMSK1 |= _BV(OCIE1A);
}

ISR(TIMER1_COMPA_vect){
	
}

int main(void){
	sei();
	init_timer();	
	while(1){				
	}
}
